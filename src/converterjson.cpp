#include "converterjson.h"
#include <nlohmann/json.hpp>
#include <fstream>

//Проверка на наличие конфигурационного файла!
void ConverterJSON::CheckConfig() {
    nlohmann::json dict;
    std::ifstream read_file_config;
    read_file_config.open("config.json");

    if(!read_file_config.is_open()) {
        throw NotOpenFile();
    }
    else {
        read_file_config >> dict;
        read_file_config.close();
        if(!dict.count("config")) {
            throw MissingKey();
        }
    }
}


//Получение текстовых данных из документов
//Каждая ячейка вектора тестовые данные одного документа
std::vector<std::string> ConverterJSON::GetTextDocument() const{
    nlohmann::json dict;
    std::ifstream read_file_doc, read_file_text;
    std::vector<std::string> text_doc;
    read_file_doc.open("config.json");

    if (read_file_doc.is_open()) {
        read_file_doc >> dict;
        read_file_doc.close();
    }
    else {
        throw NotOpenFile();
    }
    if(dict.count("files")) {
        for(auto& d : dict["files"]) {
            std::string tempo_text;
            read_file_text.open(d);
            std::getline(read_file_text, tempo_text);
            text_doc.push_back(tempo_text);
            read_file_text.close();
        }
    }
    else {
        throw MissingKey();
    }
    
    return text_doc;
}


//Получение кол-ва необходимых ответов на запрос
int ConverterJSON::GetResponsesLimit() const{
    nlohmann::json dict;
    std::ifstream read_file_resp;
    int max_resp = 0;
    read_file_resp.open("config.json");

    if(read_file_resp.is_open()) {
        read_file_resp >> dict;
        read_file_resp.close();
    }
    else {
        throw NotOpenFile();
    }
    if(dict["config"].count("max_responses")) {
        max_resp = dict["config"]["max_responses"];
    }
    else {
        throw MissingKey();
    }
    
    return max_resp;    
}


//Получение текста запросов из файла
//В каждой ячейке несколько слов
std::vector<std::string> ConverterJSON::GetRequests() const{ 
    nlohmann::json dict;
    std::ifstream read_file_request;
    std::vector<std::string> requests;
    read_file_request.open("request.json");

    if(read_file_request.is_open()) {
        read_file_request >> dict;
        read_file_request.close();
    }
    else {
        throw NotOpenFile();
    }
    if (dict.count("requests")) {
        for(int i = 0; i < dict["requests"].size(); i++) {
            requests.push_back(dict["requests"][i]);
        }
       
    }
    else {
        throw MissingKey();
    }

    return requests;
}

//Запись готового ответа в файл
void ConverterJSON::PutAnswer(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    const auto limit = GetResponsesLimit();
    nlohmann::json dict;
    std::ofstream write_answer;
    write_answer.open("answer.json");
    write_answer.clear();
    std::string request;
    
    for(int i = 0; i < answers.size(); i++) {
        if(i <= 1000) {
            request = "request " + std::to_string(i);
        }
        else {
            std::cout << "The request limit has been exceeded!" << std::endl;
            break;
        }
        if(answers[i][0].second == 0) {
            dict["answers"][request]["result"] = false;
        }
        else {
            dict["answers"][request]["result"] = true;
            for(const auto& it: answers[i]) {
                if(it.second != 0) {
                    dict["answers"][request]["relevance"].push_back({{"DocId", it.first}, {"relative", it.second}});
                }
            }
        }
        
    }
    
    write_answer << std::setw(3) << dict;

    write_answer.close();
}