#include <catch2/catch_test_macros.hpp>
#include <list>
#include <array>
#include "src/ds/list.h"
using namespace std;

namespace DS {

typedef Utils::linked_list<int> LinkedList;

static void alternative_merge_linked_lists(LinkedList& l1, LinkedList& l2) {
	LinkedList::node *n1 = const_cast<LinkedList::node*>(l1.head());

	while (n1) {
		l1.insert_after(n1,const_cast<LinkedList::node*>(l2.detach_node_from_list(l2.head())));
		n1 = n1->next->next;
	}
}

TEST_CASE("List : alternative merge test 1") {
	LinkedList l1(new LinkedList::node(new LinkedList::node(new LinkedList::node(new LinkedList::node(new LinkedList::node(11), 13), 17), 7), 5));
	LinkedList l2(new LinkedList::node(new LinkedList::node(new LinkedList::node(new LinkedList::node(new LinkedList::node(6), 4), 2), 10), 12));
	alternative_merge_linked_lists(l1, l2);

	const array<int,10> expected = {5,12,7,10,17,2,13,4,11,6};
	array<int, 10> result;

	REQUIRE((l2.head() == nullptr && l2.tail() == nullptr));
	LinkedList::node* n = const_cast<LinkedList::node*>(l1.head());
	int i = -1;
	while (n) {
		result[++i] = n->payload;
		n = n->next;
	}
	REQUIRE(expected == result);
}


}