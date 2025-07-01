#pragma once
#include <iostream>

#include "invertedindex.h"


struct RelativeIndex {
    size_t doc_id;
    float rank = 0.0f;

    bool operator == (const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};


class SearchServer {
    InvertedIndex _index;

public:
    //idx в конструктор класса передается ссылка на класс InverterIndex
    //Чтобы SearchServer мог узнать частоту слов встречающихся в запросе
    SearchServer(InvertedIndex& idx) : _index(idx){};

    int SumCount(const std::vector<Entry>& num) const;
    //Метод обработки поисковых запросов
    //queries_input поисковые запросы взятые из файла request.json
    //Возвращает отсортированный список релевантных ответов для заданных запросов
    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::vector<std::string>>& queries_input) const;

    std::vector<std::vector<std::string>> PreRequest(const std::vector<std::string>& request_list) const;

    std::vector<std::vector<std::pair<int, float>>> PreAnswer(const std::vector<std::vector<RelativeIndex>>& search_result) const;
};