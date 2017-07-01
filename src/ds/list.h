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

	linked_list() :_tail(nullptr), _head(nullptr) {}
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

 	node* detach_node_from_list(node *n){ //but not free it
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
	inline const node* tail() { return _tail; }
	inline const node* head() { return _head; }
private:
	node* _tail; // double linked list tail pointer
	node* _head; // double linked list head pointer
};
    
}
#endif
