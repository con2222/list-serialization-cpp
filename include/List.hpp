#pragma once

#include <cstdint>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

class List {
public:
	List();
	

	ListNode* push_back(std::string data);

	void add_links(std::vector<ListNode*>& nodes, std::vector<int32_t>& randIndexes);
	bool serialize_list(const std::unordered_map<ListNode*, int32_t>& randIndexMap, std::string_view filename);
	bool deserialize_list(std::string_view& filename);
	void clear_list();

	bool save_to_text(std::string_view filename) const;
	void print_list(ListNode* head) const;

	~List();

private:
	ListNode* head;
	ListNode* tail;

	int32_t find_node_index(ListNode* currentNode, const std::unordered_map<ListNode*, int32_t>& randIndexMap) const;
};