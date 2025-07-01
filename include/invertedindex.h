#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

struct Entry {
    size_t doc_id = 0;
    size_t count = 0;
    
    //Данный оператор необходим для проведения тестовых сценариев
    bool operator == (const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};              


class InvertedIndex {
    
    Entry _entry;
    std::vector<std::string> docs;
    
    std::map<std::string, std::vector<Entry>> freq_dictionary;

public:
    InvertedIndex() = default;

    //Обновить или заполнить базу документов, по которой будем совершать поиск,
    void UpdateDocumentBase(std::string input_docs, int num_doc);

    void StartThread(const std::vector<std::string>& documents);
    //Метод определяет кол-во вхождений искомого слова в загруженной базе документов.
    //Word слово, частоту вхождений которого необходимо определить
    //Возвращает подготовленный список с частотой слов 
    std::vector<Entry> GetWordCount(const std::string& word) const;

    void ClearFreqDictionary();
};