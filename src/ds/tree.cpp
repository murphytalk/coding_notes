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
struct TreeNode {
	TreeNode* left;
	TreeNode* right;
	T data;
	TreeNode(TreeNode *l, TreeNode *r, T d) :left(l), right(r), data(d) {}
	TreeNode(T d) :TreeNode(nullptr, nullptr, d) {}
	~TreeNode() { LOG << "Deleted tree node " << data <<endl; }
};


/*
           1
		2     3
      4   5      6
	 7   8 9   10
*/

static TreeNode<int>* make_tree() {
	auto root = new TreeNode<int>(1);
	auto n2 = new TreeNode<int>(2);
	auto n3 = new TreeNode<int>(3);
	auto n4 = new TreeNode<int>(4);
	auto n5 = new TreeNode<int>(5);

	root->left = n2;
	root->right = n3;

	n2->left = n4;
	n2->right = n5;
	n3->right= new TreeNode<int>(6);
	n3->right->left= new TreeNode<int>(10);

	n4->left = new TreeNode<int>(7);
	n5->left = new TreeNode<int>(8);
	n5->right= new TreeNode<int>(9);

	return root;
}

static void free_tree(TreeNode<int>* root) {
	if (root == nullptr) return;
	if (root->left) free_tree(root->left);
	if (root->right) free_tree(root->right);
	delete root;
}

static void inorder_DFS_recursion(TreeNode<int>* node, function<void(int)> output){
	if (node->left)  inorder_DFS_recursion(node->left ,output);
	output(node->data);
	if (node->right) inorder_DFS_recursion(node->right,output);
}

static void inorder_DFS_stack(TreeNode<int>* node, function<void(int)> output) {
	stack<TreeNode<int>*> s;

	auto n = node;
	while (n != nullptr) {
		s.push(n);
		n = n->left;
	}
	while (s.size() > 0) {
		n = s.top();
		s.pop();
		output(n->data);
		if (n->right) output(n->right->data);
	}
}

template<typename T>
struct Output {
	ostringstream s;
	void operator()(T data) {s << data << " ";}
};

TEST_CASE("Tree : inorder DFS"){
	TreeNode<int>* root = make_tree();
	char expected[] = "7 4 8 5 2 1 3 10 6 ";

	SECTION("Use recursion") {
		Output<int> output;
		inorder_DFS_recursion(root, ref(output));
		REQUIRE(output.s.str() == expected );
	}

	SECTION("Use stack") {
		Output<int> output;
		inorder_DFS_stack(root, ref(output));
		REQUIRE(output.s.str() == expected);
	}


	free_tree(root);
}

static void BFS_deque(TreeNode<int>* node, function<void(int)> output) {
	deque<TreeNode<int>*> q;
	q.push_back(node);


	if (node->left) q.push_back(node->left);
	if (node->right) q.push_back(node->right);
	node = node->left;
}

static void BFS_recursion(TreeNode<int>* node, function<void(int)> output) {
	//use a local functor class to simulate inner function
	struct bfs {
		typedef enum {LEFT=0,RIGHT=10} POSITION;
		//(level, accumulated position,node)
		typedef tuple<int, int, TreeNode<int>*> NODE;
		vector<NODE> nodes;
		void operator()(int level,int pos,TreeNode<int>* node) {
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
	TreeNode<int>* root = make_tree();
	char expected[] = "1 2 3 4 5 6 7 8 9 10 ";
	
	SECTION("Use recursion") {
		Output<int> output;
		BFS_recursion(root, ref(output));
		REQUIRE(output.s.str() == expected);
	}

	free_tree(root);
}

}