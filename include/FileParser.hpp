#pragma once

#include "List.hpp"

// std
#include <string_view>
#include <unordered_map>

bool populate_list_from_file(List& list, std::unordered_map<ListNode*, int32_t>& randIndexMap, std::string_view filename);