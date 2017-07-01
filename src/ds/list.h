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
        node(node* n, T py):next(n),prev(nullptr), payload(py) {};
		node(node* p, node* n, T py) :node(n, py) { prev = p; };
        node(T p):node(nullptr, nullptr, p){}
    };

	linked_list() :_tail(nullptr), _head(nullptr) {}
	linked_list(node* h):_head(h){
		node* n = h;
		node* p = nullptr;
		while (n) {
			if (p && n->prev == nullptr) n->prev = p;
			p = n;
			n = n->next;
		}
		_tail = p;
	}
	~linked_list() {
		if (_head) {
			node* cur = _head;
			node* next;
			while (cur) {
				next = cur->next;
				delete cur;
				cur = next;
			}
		}
	}

 	const node* detach_node_from_list(const node *n){ //but not free it
		if (!n) return nullptr;
		node* prev = n->prev;
		node* next = n->next;
		if (prev) {
			prev->next = next;
		}
		if (next) {
			next->prev = prev;
		}
		if (n == _head) {
			_head = next;
		}
		if (n == _tail) {
			_tail = prev;
		}
		return n;
	}

	inline void delete_node(node* n) {
		delete detach_node_from_list(n);
	}

	inline void delete_tail() {
		delete_node(_tail);
	}

	void add_to_head(node *n) {
		if (_head) _head->prev = n;
		n->prev = nullptr;
		n->next = _head;
		_head = n;
		if (_tail == nullptr) _tail = n;
	} 

	void move_to_head(node *same) {
		detach_node_from_list(same);
		add_to_head(same);
	}

	void insert_after(node* after, node* new_node) {
		node* next = after->next;
		after->next = new_node;
		new_node->prev = after;
		new_node->next = next;
		if (next) next->prev = new_node;
	}

	inline const node* tail() { return _tail; }
	inline const node* head() { return _head; }
private:
	node* _tail; // double linked list tail pointer
	node* _head; // double linked list head pointer
};
    
}
#endif
