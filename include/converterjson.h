#pragma once
#include <iostream>
#include <vector>
#include <exception>

class MissingKey: public std::exception {
    public:
    const char* what() const noexcept override {
        return "The key is missing from the file! Error in request.json";
    }
};

class NotOpenFile: public std::exception {
    public:
    const char* what() const noexcept override {
        return "The file config.json was not opened.";
    }
};



class ConverterJSON
{
    
public:
    ConverterJSON() = default;
    /*
        Метод получения содержимого файлов
        Возвращает список с содержимым файлов пречисленных в config.json
    */
    std::vector<std::string> GetTextDocument() const;

    /*
        Метод считывает поле max_responsed для определения
        предельного количества ответов на один запрос
    */
    int GetResponsesLimit() const;

    /*
        Метод получения запросов из файла requests.json
        возвращает список запросов из файла request.json
    */
    std::vector<std::string> GetRequests() const;

    /*
        Положить в файл answer.json результаты поисковых запросов
    */
    void PutAnswer(const std::vector<std::vector<std::pair<int, float>>>& answers);

    // Проверка наличия файла config.json перед запуском программы
    void CheckConfig();
};