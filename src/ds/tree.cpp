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
class tree{
public:
    struct node{
        node* left;
        node* right;
        T data;
        node(node *l, node *r, T d) :left(l), right(r), data(d) {}
        node(T d) :node(nullptr, nullptr, d) {}
        ~node() { LOG << "Deleted tree node " << data <<endl; }
    };

    node* root;
    tree(node* r = nullptr) :root(r) {}
    ~tree(){
        free_tree(root);
        LOG << "Tree deleted" << endl;
    }
private:
    void free_tree(node* node){
        if (node == nullptr) return;
        if (node->left) free_tree(node->left);
        if (node->right) free_tree(node->right);
        delete node;

    }
};

template<typename T>
class BST : public tree<T> {
	typedef typename tree<T>::node node; //gcc and clang needs this
	node* _insert(node* n,const T data) {
		if (n == nullptr) return new node(data);
		if (data < n->data) {
			n->left = _insert(n->left, data);
		}
		else if (data > n->data) {
			n->right= _insert(n->right, data);
		}
		return n;
	}
	
	node* _search(node* n, const T data) {
		if (n == nullptr) return nullptr;
		if (data < n->data) {
			return _search(n->left, data);
		}
		else if (data > n->data) {
			return _search(n->right, data);
		}
		else return n;
	}
public:
	node* insert(const T data) {
		//gcc needs "this" pointer to access parent template class member ...
		//vc is more forgiving
		//see https://stackoverflow.com/questions/6592512/templates-parent-class-member-variables-not-visible-in-inherited-class
		auto p =_insert(this->root,data);
		if (this->root == nullptr) this->root = p;
		return p;
	}

	void del(const T data) {
		auto p = search(data);
		if (p) {

		}
	}

	node* search(const T data) {
		return _search(this->root, data);
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

static tree<int>::node* create_tree() {
    auto root = new tree<int>::node(1);
    auto n2 = new tree<int>::node(2);
    auto n3 = new tree<int>::node(3);
    auto n4 = new tree<int>::node(4);
    auto n5 = new tree<int>::node(5);

    root->left = n2;
    root->right = n3;

    n2->left = n4;
    n2->right = n5;
    n3->right= new tree<int>::node(6);
    n3->right->left= new tree<int>::node(10);

    n4->left = new tree<int>::node(7);
    n5->left = new tree<int>::node(8);
    n5->right= new tree<int>::node(9);

    return root;
}

static void inorder_DFS_recursion(tree<int>::node* node, function<void(int)> output){
    if (node->left)  inorder_DFS_recursion(node->left ,output);
    output(node->data);
    if (node->right) inorder_DFS_recursion(node->right,output);
}

static void inorder_DFS_stack(tree<int>::node* node, function<void(int)> output) {
    stack<tree<int>::node*> s;

    auto push_left = [&s](tree<int>::node* n){
            while (n != nullptr) {
                s.push(n);
                n = n->left;
            }
    };
    
    auto n = node;
    push_left(n);
    while (s.size() > 0) {
        n = s.top();
        output(n->data);
        s.pop();
        if (n->right) {
            push_left(n->right);
        }
    }
}

template<typename T>
struct Output {
    ostringstream s;
    void operator()(T data) {s << data << " ";}
};

TEST_CASE("Tree : inorder DFS"){
    const tree<int> tree(create_tree());
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

static void BFS_deque(tree<int>::node* node, function<void(int)> output) {
    deque<tree<int>::node*> q;

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

static void BFS_recursion(tree<int>::node* node, function<void(int)> output) {
    struct bfs {
        enum {LEFT=0,RIGHT=10};
        //(level, accumulated position,node)
        typedef tuple<int, int, tree<int>::node*> NODE;
        vector<NODE> nodes;
        void operator()(int level,int pos,tree<int>::node* node) {
            nodes.push_back(make_tuple(level++,pos,node));
            if (node->left) (*this)(level,pos+LEFT,node->left);
            if (node->right) (*this)(level,pos+RIGHT,node->right);
        }
    };

    bfs b;
    b(1,0,node);
    
    //sort by the following priority : level and then position. 
    //note position is accumulated, so for the nodes on the same level, 
	//the node whose parent is its greatparent's left child would have higher accumulated position value
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
    const tree<int> tree(create_tree());
    const char expected[] = "1 2 3 4 5 6 7 8 9 10 ";
    
    SECTION("Use recursion") {
        Output<int> output;
        BFS_recursion(tree.root, ref(output));
        REQUIRE(output.s.str() == expected);
    }

    SECTION("Use deque") {
        Output<int> output;
        BFS_deque(tree.root, ref(output));
        REQUIRE(output.s.str() == expected);
    }
}

/*
           1                          1
         /   \                      /   \
        2     3                    3     2
       / \     \                 /      / \ 
      4   5     6      ==>      6      5   4 
     /   / \    /                \    / \   \
    7   8   9  10                10  9   8   7 
*/
TEST_CASE("Tree : mirror") {
	struct mirror {
		void operator()(tree<int>::node* node) {
			if (node == nullptr) return;
			(*this)(node->left);
			(*this)(node->right);
			auto temp = node->left;
			node->left = node->right;
			node->right = temp;
		}
	};

	tree<int> tree(create_tree());

	mirror m;
	m(tree.root);


    Output<int> output;
    BFS_deque(tree.root, ref(output));
    REQUIRE(output.s.str() == "1 3 2 6 5 4 10 9 8 7 ");

	auto n6 = tree.root->left->left;
	REQUIRE(n6->data == 6);
	REQUIRE(n6->left == nullptr);
	REQUIRE( (n6->right != nullptr && n6->right->data == 10));

    Output<int> output2;
    inorder_DFS_recursion(tree.root, ref(output2));
    REQUIRE(output2.s.str() == "6 10 3 1 9 5 8 2 4 7 " );
}

TEST_CASE("Tree : BST insert/search/delete") {
/* 
	    50
	  /     \
	30      70
   /  \    /  \ 
 20   40  60   80 
*/
	BST<int> bst;
	int data[] = {50, 30, 20, 40, 70, 60, 80};
	for_each(data, data + sizeof(data) / sizeof(int), [&bst](int d) {bst.insert(d);});

	SECTION("insert") {
		Output<int> output;
		BFS_deque(bst.root, ref(output));
		REQUIRE(output.s.str() == "50 30 70 20 40 60 80 ");
	}

	SECTION("search") {
		REQUIRE(bst.search(50) == bst.root);
		REQUIRE(bst.search(30) == bst.root->left);
		REQUIRE(bst.search(20) == bst.root->left->left);
		REQUIRE(bst.search(80) == bst.root->right->right);
		REQUIRE(bst.search(99) == nullptr);
	}
#if 0
	SECTION("delete 20") {
		Output<int> output;
		bst.del(20);
		BFS_deque(bst.root, ref(output));
		REQUIRE(output.s.str() == "50 30 70 40 60 80 ");
		Output<int> output2;
		inorder_DFS_recursion(bst.root, ref(output));
		REQUIRE(output.s.str() == "30 40 50 60 70 80 ");
	}
#endif
}


}
