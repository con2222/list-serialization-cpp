#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstring>


struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

ListNode* push_back(ListNode*& head, ListNode*& tail, std::string data) {
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

int find_node_index(ListNode* currentNode, const std::unordered_map<ListNode*, int>& randIndexMap) {
    if (currentNode->rand == nullptr) {return -1;}
	return randIndexMap.at(currentNode->rand);
}

void add_links(std::vector<ListNode*>& nodes, std::vector<int>& randIndexes) {
    if (nodes.empty() || nodes.size() != randIndexes.size()) {return;}

	for (size_t i = 0; i < nodes.size(); i++) {
		if (randIndexes[i] != -1) {
			nodes[i]->rand = nodes[randIndexes[i]];
		}
	}
}

bool serialize_list(ListNode* head, const std::unordered_map<ListNode*, int>& randIndexMap) {
    if (head == nullptr) {return false;}

    int randIndex = -1;
    size_t len = 0;
    ListNode* currentNode = head;

    std::ofstream file("outlet.out", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл на запись!" << std::endl;
        return false;
    }

	std::string buffer;

    while (currentNode != nullptr) {
        len = currentNode->data.size();
        randIndex = find_node_index(currentNode, randIndexMap);
		size_t total_size = sizeof(len) + len + sizeof(randIndex);

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

bool deserialize_list(ListNode*& head, ListNode*& tail) {
	std::ifstream file("outlet.out", std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Не удалось открыть файл на чтение!" << std::endl;
        return false;
	}

	std::vector<int> randIndexes;
	std::vector<ListNode*> nodes;
	int randIndex = -1;
	size_t len = 0;

	while (file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
		std::string str;
		str.resize(len);

		file.read(&str[0], len);
		file.read(reinterpret_cast<char*>(&randIndex), sizeof(randIndex));

		randIndexes.push_back(randIndex);
		nodes.push_back(push_back(head, tail, str));
	}
	add_links(nodes, randIndexes);

	file.close();
	return true;
}

void clear_list(ListNode*& head, ListNode*& tail) {
	ListNode* currentNode = head;
	while (currentNode != nullptr) {
		ListNode* temp = currentNode;
		currentNode = currentNode->next;
		delete temp;
	}
	head = nullptr;
	tail = nullptr;
}

void print_list(ListNode* head) {
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

int main() {
    std::ifstream file("inlet.in");
	if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    std::vector<ListNode*> nodes;
	std::vector<int> randIndexes;
	std::unordered_map<ListNode*, int> randIndexMap;
	int counter = 0;

    ListNode* head = nullptr;
    ListNode* tail = nullptr;

    std::string line;
    while (std::getline(file, line)) {
		int pos = line.rfind(';');

        int n = std::stoi(&line[pos + 1]);
    	line.resize(pos);

		ListNode* newNode = push_back(head, tail, line);
        randIndexMap.insert(std::make_pair(newNode, counter++));
        nodes.push_back(newNode);
		randIndexes.push_back(n);
    }
    add_links(nodes, randIndexes);

    serialize_list(head, randIndexMap);

	clear_list(head, tail);
    file.close();

	return 0;
}
