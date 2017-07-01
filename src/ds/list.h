#ifndef _LIST_H
#define _LIST_H
namespace Utils{

template<typename T>
class linked_list{
public:
    struct node {
        node* next;
        node* prev;
        T payload;
        node(node* p, node* n, T py) :prev(p), next(n), payload(py) {};
        node(T p):node(nullptr, nullptr, p){}
    };
    
	node* tail; // double linked list tail pointer
	node* head; // double linked list head pointer

	linked_list() :tail(nullptr), head(nullptr) {}

 	node* delete_node(node *n){ //but not free it
		if (!n) return nullptr;
		node* prev = n->prev;
		node* next = n->next;
		if (prev) {
			prev->next = next;
		}
		if (next) {
			next->prev = prev;
		}
		if (n == head) {
			head = next;
		}
		if (n == tail) {
			tail = prev;
		}
		return n;
	}

	void add_to_head(node *n) {
		if (head) head->prev = n;
		n->prev = nullptr;
		n->next = head;
		head = n;
	} 

	void move_to_head(node *same) {
		delete_node(same);
		add_to_head(same);
	}
/*
	inline const node* tail() { return tail; }
	inline const node* head() { return head; }
private:
*/
};
    
}
#endif
