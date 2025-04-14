#pragma once
#include <iostream>

#include "invertedindex.h"

#include "gtest/gtest.h"

struct RelativeIndex {
    size_t doc_id = 0;
    float rank = 0.0f;

    bool operator == (const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};


class SearchServer {
    InvertedIndex _index;
    RelativeIndex _relativeindex;
    std::vector<size_t> max_rank;

public:
    //idx в конструктор класса передается ссылка на класс InverterIndex
    //Чтобы SearchServer мог узнать частоту слов встречающихся в запросе
    SearchServer(InvertedIndex& idx) : _index(idx){};

    //Суммирует данные поля count
    int count_sum(std::vector<Entry>& sum);
    
    //Исключаем повторяемые слова в запросе и сортируем его по count
    std::vector<std::vector<std::string>> request_processing(const std::vector<std::string>& request_input);

    //Выводим в консоль ответ пользователю
    void console_answer(const std::vector<std::vector<std::string>>& console);

    //Метод обработки поисковых запросов
    //queries_input поисковые запросы взятые из файла request.json
    //Возвращает отсортированный список релевантных ответов для заданных запросов
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::vector<std::string>>& queries_input);

    //Подготовка релевантных ответов для отправки в файл
    std::vector<std::vector<std::pair<int, float>>> preparing_response(const std::vector<std::vector<RelativeIndex>>& search_processing, int response_limit);
};