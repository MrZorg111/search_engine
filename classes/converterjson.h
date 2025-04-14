#pragma once
#include <iostream>
#include <vector>
#include <exception>

#include "gtest/gtest.h"

class MissingKey: public std::exception {
    public:
    const char* what() const noexcept override {
        return "The key is missing from the file!";
    }
};

class NotOpenFile: public std::exception {
    public:
    const char* what() const noexcept override {
        return "The file was not opened.";
    }
};



class ConverterJSON
{
    std::string config_adress = "../../data/config.json";
    std::string request_adress = "../../data/request.json";
    std::string answer_adress = "../../data/answer.json";

    
public:
    ConverterJSON() = default;
    /*
        Метод получения содержимого файлов
        Возвращает список с содержимым файлов пречисленных в config.json
    */
    std::vector<std::string> GetTextDocument();

    /*
        Метод считывает поле max_responsed для определения
        предельного количества ответов на один запрос
    */
    int GetResponsesLimit();

    /*
        Метод получения запросов из файла requests.json
        возвращает список запросов из файла request.json
    */
    std::vector<std::string> GetRequests();

    /*
        Положить в файл answer.json результаты поисковых запросов
    */
    void putAnswer(const std::vector<std::vector<std::pair<int, float>>>& answers);

    // Проверка наличия файла config.json перед запуском программы
    bool CheckConfig();
};
