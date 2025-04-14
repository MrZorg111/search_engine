#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "gtest/gtest.h"

struct Entry {
    size_t doc_id = 0, count = 0;
    
    //Данный оператор необходим для проведения тестовых сценариев
    bool operator == (const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};              


class InvertedIndex {
    
    Entry _entry;
    
    //Коллекция для хранения текстов документов, в которой номер элемента в векторе определяет doc_id для формирования результата запроса
    std::vector<std::string> docs;

    //Частотный словарь, для хранения частоты слов, встречаемый в тексте Entry - структура, объявлена выше.
    std::map<std::string, std::vector<Entry>> freq_dictionary;

public:
    InvertedIndex() = default;

    //Обновить или заполнить базу документов, по которой будем совершать поиск,
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    //Метод определяет кол-во вхождений искомого слова в загруженной базе документов.
    //Word слово, частоту вхождений которого необходимо определить
    //Возвращает подготовленный список с частотой слов 
    std::vector<Entry> GetWordCount(const std::string& word);

    //Возвращает общее кол-во документов
    int GetNumberDocument();

    //Возвращает статус freq_dictionary, пуст/нет
    //void GetStatFreq();

};