#include <gtest/gtest.h>
#include "List.hpp"

// std
#include <filesystem>

// Тест 1: Проверка базового добавления элементов
TEST(ListTest, PushBackAddsElements) {
    List list;
    list.push_back("Apple");
    list.push_back("Banana");

    EXPECT_TRUE(list.save_to_text("test_output.txt")); // сериализация не падает на непустом списке
}

// Тест 2: Проверка сериализации пустого списка (Edge Case)
TEST(ListTest, EmptyListSerialization) {
    List list;
    std::unordered_map<ListNode*, int32_t> emptyMap;

    EXPECT_FALSE(list.serialize_list(emptyMap, "empty.bin")); //функция должна вернуть false или корректно обработать пустой список.
}

// Тест 3: Round-trip тест (Сериализация -> Десериализация -> Сравнение)
TEST(ListTest, SerializationRoundTrip) {
    std::string bin_file = "roundtrip.bin";
    std::string text_file1 = "original.txt";
    std::string text_file2 = "restored.txt";

    List originalList;
    ListNode* node1 = originalList.push_back("First");
    ListNode* node2 = originalList.push_back("Second");
    ListNode* node3 = originalList.push_back("Third");

    node1->rand = node3;
    node2->rand = node1;
    node3->rand = nullptr;

    originalList.save_to_text(text_file1);

    std::unordered_map<ListNode*, int32_t> map = {
        {node1, 0}, {node2, 1}, {node3, 2}
    };
    ASSERT_TRUE(originalList.serialize_list(map, bin_file));

    List restoredList;
    std::string_view bin_view = bin_file;
    ASSERT_TRUE(restoredList.deserialize_list(bin_view));

    restoredList.save_to_text(text_file2);

    auto size1 = std::filesystem::file_size(text_file1);
    auto size2 = std::filesystem::file_size(text_file2);

    EXPECT_EQ(size1, size2) << "Размеры текстовых дампов не совпадают!";

    std::filesystem::remove(bin_file);
    std::filesystem::remove(text_file1);
    std::filesystem::remove(text_file2);
}