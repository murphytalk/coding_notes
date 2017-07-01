#include "catch.hpp"
#include <stack>
#include <deque>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include "../utils/utils.h"

using namespace std;

namespace DS{

template<typename T>
class Tree{
public:
    struct Node{
    	Node* left;
    	Node* right;
    	T data;
    	Node(Node *l, Node *r, T d) :left(l), right(r), data(d) {}
    	Node(T d) :Node(nullptr, nullptr, d) {}
    	~Node() { LOG << "Deleted tree node " << data <<endl; }
    };

	Node* root;
	Tree(Node* r) :root(r) {}
	~Tree(){
		free_tree(root);
		LOG << "Tree deleted" << endl;
	}
private:
	void free_tree(Node* node){
		if (node == nullptr) return;
		if (node->left) free_tree(node->left);
		if (node->right) free_tree(node->right);
		delete node;

	}
};

/*
           1
		 /   \
		2     3
	   / \     \
      4   5     6
	 /   / \    / 
	7   8   9  10
*/

static Tree<int>::Node* create_tree() {
	auto root = new Tree<int>::Node(1);
	auto n2 = new Tree<int>::Node(2);
	auto n3 = new Tree<int>::Node(3);
	auto n4 = new Tree<int>::Node(4);
	auto n5 = new Tree<int>::Node(5);

	root->left = n2;
	root->right = n3;

	n2->left = n4;
	n2->right = n5;
	n3->right= new Tree<int>::Node(6);
	n3->right->left= new Tree<int>::Node(10);

	n4->left = new Tree<int>::Node(7);
	n5->left = new Tree<int>::Node(8);
	n5->right= new Tree<int>::Node(9);

	return root;
}

static void inorder_DFS_recursion(Tree<int>::Node* node, function<void(int)> output){
	if (node->left)  inorder_DFS_recursion(node->left ,output);
	output(node->data);
	if (node->right) inorder_DFS_recursion(node->right,output);
}

static void inorder_DFS_stack(Tree<int>::Node* node, function<void(int)> output) {
	stack<Tree<int>::Node*> s;

	struct push_left {
		stack<Tree<int>::Node*>& s;
		push_left(stack<Tree<int>::Node*>& _s) :s(_s) {}
		void operator()(Tree<int>::Node* n){
			while (n != nullptr) {
				s.push(n);
				n = n->left;
			}
		}
	};
	
	push_left p(s);
	
	auto n = node;
	p(n);
	while (s.size() > 0) {
		n = s.top();
		output(n->data);
		s.pop();
		if (n->right) {
			p(n->right);
		}
	}
}

template<typename T>
struct Output {
	ostringstream s;
	void operator()(T data) {s << data << " ";}
};

TEST_CASE("Tree : inorder DFS"){
	const Tree<int> tree(create_tree());
	const char expected[] = "7 4 2 8 5 9 1 3 10 6 ";

	SECTION("Use recursion") {
		Output<int> output;
		inorder_DFS_recursion(tree.root, ref(output));
		REQUIRE(output.s.str() == expected );
	}

	SECTION("Use stack") {
		Output<int> output;
		inorder_DFS_stack(tree.root, ref(output));
		REQUIRE(output.s.str() == expected);
	}
}

static void BFS_deque(Tree<int>::Node* node, function<void(int)> output) {
	deque<Tree<int>::Node*> q;

	auto n = node;
	while (n != nullptr) {
		output(n->data);
		if (n->left)  q.push_back(n->left);
		if (n->right) q.push_back(n->right);
		if (q.empty()) {
			n = nullptr;
		}
		else {
			n = q.front();
			q.pop_front();
		}
	}
}

static void BFS_recursion(Tree<int>::Node* node, function<void(int)> output) {
	//use a local functor class to simulate inner function
	struct bfs {
		enum {LEFT=0,RIGHT=10};
		//(level, accumulated position,node)
		typedef tuple<int, int, Tree<int>::Node*> NODE;
		vector<NODE> nodes;
		void operator()(int level,int pos,Tree<int>::Node* node) {
			nodes.push_back(make_tuple(level++,pos,node));
			if (node->left) (*this)(level,pos+LEFT,node->left);
			if (node->right) (*this)(level,pos+RIGHT,node->right);
		}
	};

	bfs b;
	b(1,0,node);
	
	//sort by the following priority : level and then position. 
	//note position is accumulated, so for the nodes on the same level, the node whose parent is its greatparent's left child would have higher accumulated position value
	sort(b.nodes.begin(), b.nodes.end(), [](const bfs::NODE& n1, const bfs::NODE& n2) {
		const int n1_level = get<0>(n1);
		const int n2_level = get<0>(n2);
		if (n1_level < n2_level)
			return true;
		else if (n1_level == n2_level) {
			int n1_pos = get<1>(n1);
			int n2_pos = get<1>(n2);
			return n1_pos < n2_pos;
		}
		return false;
	});
	
	for_each(b.nodes.begin(), b.nodes.end(), [&output](const bfs::NODE& n) {output(get<2>(n)->data); });
}


TEST_CASE("Tree : BFS"){
	const Tree<int> forrest(create_tree());
	const char expected[] = "1 2 3 4 5 6 7 8 9 10 ";
	
	SECTION("Use recursion") {
		Output<int> output;
		BFS_recursion(forrest.root, ref(output));
		REQUIRE(output.s.str() == expected);
	}

	SECTION("Use deque") {
		Output<int> output;
		BFS_deque(forrest.root, ref(output));
		REQUIRE(output.s.str() == expected);
	}
}

}
