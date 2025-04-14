#include "searchserver.h"
#include <vector>
#include <sstream>

int SearchServer::count_sum(std::vector<Entry>& sum) {
    int sum_count = 0;
    for(const auto& it: sum) {
       sum_count +=  it.count; 
    }

    return sum_count;
}

std::vector<std::vector<std::string>> SearchServer::request_processing(const std::vector<std::string>& request_input) {
std::vector<std::vector<std::string>> requires_list;
    std::vector<std::string> requires;
    //Разобьем запросы на отдельные слова 
    //Исключим повторы в каждом отдельном запросе
    for(int i = 0; i < request_input.size(); i++) {
        std::istringstream iss(request_input[i]);
        std::string tempo_word;
        
        while(iss >> tempo_word) {
            if(requires.size() > 0) {
                for(int t = 0; t < requires.size(); t++) {
                    if(tempo_word == requires[t]) {
                        break;
                    }
                    else if(t == requires.size() - 1) {
                        requires.push_back(tempo_word);
                    }
                }
            }
            else {
                requires.push_back(tempo_word);
            }
            tempo_word.clear();
        }
        requires_list.push_back(requires);
        requires.clear();
    }
    
    //Отсортируем в порядке возрастания встречаемости
    for(int i = 0; i < requires_list.size(); i++) {
        int tempo_rank = 0;
        for(int t = 0; t < requires_list[i].size() - 1; t++) {
            for(int j = t + 1; j < requires_list[i].size(); j++) {
                auto sorti_f = _index.GetWordCount(requires_list[i][t]);
                auto sorti_s = _index.GetWordCount(requires_list[i][j]);

                if (count_sum(sorti_f) > count_sum(sorti_s)) {
                    tempo_rank = count_sum(sorti_f);
                    std::string tempo = requires_list[i][t];
                    requires_list[i][t] = requires_list[i][j];
                    requires_list[i][j] = tempo;
                }
                else {
                    tempo_rank = count_sum(sorti_s);
                }
            }
        }
        max_rank.push_back(tempo_rank);
    }
    
    return requires_list;
}

void SearchServer::console_answer(const std::vector<std::vector<std::string>>& console) {
    for(int i = 0; i < console.size(); i++) {
        std::cout << "Request" + std::to_string(i) << std::endl;
        for(int t = 0; t < console[i].size(); t++) {
            std::cout << console[i][t] << ":" << std::endl;
            auto tempo = _index.GetWordCount(console[i][t]);
            for(const auto& it: tempo) {
                if(it.doc_id == 0 && it.count == 0) {
                    std::cout << "\tThere is no search word in the documents!" << std::endl;
                    break;
                }
                else {
                std::cout << "\tDocID " << it.doc_id << "\tCount " << it.count << std::endl;
                }
            }
        }
        std::cout << "\n\n" << std::endl;
    }
}


std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::vector<std::string>>& queries_input) {
    std::vector<std::vector<RelativeIndex>> relative_requires;
    std::vector<RelativeIndex>  relative;
    
    for(int i = 0; i < queries_input.size(); i++) {
        for(int d = 0; d < _index.GetNumberDocument(); d++) {
            _relativeindex.doc_id = d;
            for(int w = 0; w < queries_input[i].size(); w++) {
                const auto tempo = _index.GetWordCount(queries_input[i][w]);

                for(const auto it: tempo) {
                    if (it.doc_id == 0 && it.count == 0) {
                        break;
                    }
                    else if(it.doc_id == d){
                        _relativeindex.rank += it.count;
                    }
                }
            }
                if(max_rank[i] != 0) {
                    _relativeindex.rank = (_relativeindex.rank / max_rank[i]);
                } 
                else {
                    _relativeindex.rank = 0;
                }   
            
            relative.push_back(_relativeindex);
            
        }
        relative_requires.push_back(relative);
        relative.clear();
    }

    return relative_requires;
}

std::vector<std::vector<std::pair<int, float>>> SearchServer::preparing_response(const std::vector<std::vector<RelativeIndex>>& search_processing, int response_limit) {
    auto edit = search_processing;
    std::vector<std::vector<std::pair<int, float>>> prepared_response;
    std::vector<std::pair<int, float>> resp;
    std::pair<int, float> rel;
    
    if(_index.GetNumberDocument() < response_limit) {
        response_limit = _index.GetNumberDocument();
    }
    //Отсортируем по уменьшению релевантности
    for(int i = 0; i < edit.size(); i++) {
        for(int j = 0; j < edit[i].size() - 1; j++) {    
            for(int s = j + 1; s < edit[i].size(); s++) {
                if(edit[i][j].rank < edit[i][s].rank) {
                    auto tempo = edit[i][j];
                    edit[i][j] = edit[i][s];
                    edit[i][s] = tempo;
                }
            }
        }
    } 
    
    
    //Соберем готовый ответ
    for(int i = 0; i < edit.size(); i++) {
        for(int t = 0; t < response_limit; t++) {
            rel.first = edit[i][t].doc_id;
            rel.second = edit[i][t].rank;
            resp.push_back(rel);
        }
        prepared_response.push_back(resp);
        resp.clear();
    }
    return prepared_response;
}