#include "List.hpp"

// std
#include <iostream>
#include <fstream>

List::List() : head(nullptr), tail(nullptr) {}

ListNode* List::push_back(std::string data) {
    if (head == nullptr && tail == nullptr) {
        head = new ListNode();
        head->data = data;
        tail = head;
        return head;
    }

    ListNode* newNode = new ListNode();
    newNode->data = data;
    newNode->prev = tail;
    tail->next = newNode;
    tail = newNode;
	return newNode;
}

void List::add_links(std::vector<ListNode*>& nodes, std::vector<int32_t>& randIndexes) {
    if (nodes.empty() || nodes.size() != randIndexes.size()) {return;}

	for (uint32_t i = 0; i < nodes.size(); i++) {
		if (randIndexes[i] != -1) {
			nodes[i]->rand = nodes[randIndexes[i]];
		}
	}
}

bool List::serialize_list(const std::unordered_map<ListNode*, int32_t>& randIndexMap, std::string_view filename) {
    if (head == nullptr) {return false;}

    int32_t randIndex = -1;
    uint32_t len = 0;
    ListNode* currentNode = head;

    std::ofstream file(filename.data(), std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл на запись!" << std::endl;
        return false;
    }

	std::string buffer;

    while (currentNode != nullptr) {
        len = currentNode->data.size();
        randIndex = find_node_index(currentNode, randIndexMap);
		uint32_t total_size = sizeof(len) + len + sizeof(randIndex);

		buffer.resize(total_size);
		std::memcpy(&buffer[0], &len, sizeof(len));
		std::memcpy(&buffer[sizeof(len)], currentNode->data.c_str(), len);
		std::memcpy(&buffer[sizeof(len) + len], &randIndex, sizeof(randIndex));
		file.write(buffer.data(), total_size);

        currentNode = currentNode->next;
    }

    file.close();
    return true;
}

bool List::deserialize_list(std::string_view& filename) {
	std::ifstream file(filename.data(), std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Не удалось открыть файл на чтение!" << std::endl;
        return false;
	}

	std::vector<int32_t> randIndexes;
	std::vector<ListNode*> nodes;
	int32_t randIndex = -1;
	uint32_t len = 0;

	while (file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
		std::string str;
		str.resize(len);

		file.read(&str[0], len);
		file.read(reinterpret_cast<char*>(&randIndex), sizeof(randIndex));

		randIndexes.push_back(randIndex);
		nodes.push_back(push_back(str));
	}
	add_links(nodes, randIndexes);

	file.close();
	return true;
}

void List::clear_list() {
	ListNode* currentNode = head;
	while (currentNode != nullptr) {
		ListNode* temp = currentNode;
		currentNode = currentNode->next;
		delete temp;
	}
	head = nullptr;
	tail = nullptr;
}

bool List::save_to_text(std::string_view filename) const {
    if (head == nullptr) { return false; }

    std::unordered_map<ListNode*, int32_t> nodeToIndex;
    ListNode* current = head;
    int32_t index = 0;
    while (current != nullptr) {
        nodeToIndex[current] = index++;
        current = current->next;
    }

    std::ofstream file(filename.data());
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл для сохранения текста!" << std::endl;
        return false;
    }

    current = head;
    while (current != nullptr) {
        file << current->data << ";";
        
        if (current->rand != nullptr) {
            file << nodeToIndex.at(current->rand);
        } else {
            file << "-1"; 
        }
        
        file << "\n";
        current = current->next;
    }

    file.close();
    return true;
}

void List::print_list(ListNode* head) const {
	ListNode* currentNode = head;
	while (currentNode != nullptr) {
		if (currentNode->rand != nullptr) {
			std::cout << currentNode->data << " -> " << currentNode->rand->data << std::endl;
		} else {
			std::cout << currentNode->data << " -> NULL" << std::endl;
		}
		currentNode = currentNode->next;
	}
}

int32_t List::find_node_index(ListNode* currentNode, const std::unordered_map<ListNode*, int32_t>& randIndexMap) const {
    if (currentNode->rand == nullptr) {return -1;}
	return randIndexMap.at(currentNode->rand);
}

List::~List() {
	clear_list();
}