#include "searchserver.h"
#include <vector>
#include <sstream>

int SearchServer::SumCount(const std::vector<Entry>& num) const{
    int result = 0;

    for(const auto& it: num) {
        result += it.count;
    }
    return result;
}

std::vector<std::vector<std::string>> SearchServer::PreRequest(const std::vector<std::string>& request_list) const{
    std::vector<std::string> preparation;
    std::vector<std::vector<std::string>> result;
    

    for(int i = 0; i < request_list.size(); i++) {
        std::stringstream word(request_list[i]);
        std::string tempo_word;
        while(word >> tempo_word) {
            bool flag = true;
            for(const auto it: preparation) {
                if(it == tempo_word) {
                    flag = false;
                }
            }
            if(flag) {
                preparation.push_back(tempo_word);
            }
        }
        
        for(int t = 0; t < preparation.size(); t++) {
            for(int j = t + 1; j < preparation.size(); j++) {
                const auto& t1 = _index.GetWordCount(preparation[t]);
                const auto& t2 = _index.GetWordCount(preparation[j]);
               
                if(SumCount(t1) > SumCount(t2)) {
                    auto tempo = preparation[t];
                    preparation[t] = preparation[j];
                    preparation[j] = tempo;
                }
            }
        }
        result.push_back(preparation);
        preparation.clear();
    }
    return result;
}

std::vector<std::vector<RelativeIndex>> SearchServer::Search(const std::vector<std::vector<std::string>>& queries_input) const{
    int max_rank = 0;
    RelativeIndex _relative;
    std::vector<RelativeIndex> list_relative;
    std::vector<std::vector<RelativeIndex>> result;
    
    for(int i = 0; i < queries_input.size(); i++) {                   
        for(int t = 0; t < queries_input[i].size(); t++) {                  
            auto tempo_name = _index.GetWordCount(queries_input[i][t]);     
            for(int j = 0; j < tempo_name.size(); j++) {
                if(tempo_name[j].count == 0) {
                     _relative.doc_id = 0;  
                      _relative.rank = 0;
                      list_relative.push_back(_relative);                                               
                }
                else {
                    _relative.doc_id = tempo_name[j].doc_id;                
                    _relative.rank = tempo_name[j].count;

                    if(list_relative.empty()) {                             
                        list_relative.push_back(_relative);                 
                    }

                    else {
                        bool flag = true;
                        for(int m = 0; m < list_relative.size(); m++) {
                            if(list_relative[m].doc_id == _relative.doc_id) {
                                list_relative[m].rank += _relative.rank;
                                
                                if(list_relative[m].rank > max_rank) {
                                    max_rank = list_relative[m].rank;
                                }

                                flag = false;
                            }
                        }
                        if(flag) {
                            list_relative.push_back(_relative);
                        }
                    }
                }
            }
        }

        for(int y = 0; y < list_relative.size(); y++) {
            list_relative[y].rank /= max_rank;
        }
        
        result.push_back(list_relative);
        list_relative.clear();
    }

    for(int q = 0; q < result.size(); q++) {
        for(int w = 0; w < result[q].size(); w++) {
            for(int e = w + 1; e < result[q].size(); e++) {
                if(result[q][w].rank < result[q][e].rank) {
                    auto tempo = result[q][w];
                    result[q][w] = result[q][e];
                    result[q][e] = tempo;
                }
            }
        }
    }
    return result;
}

std::vector<std::vector<std::pair<int, float>>> SearchServer::PreAnswer(const std::vector<std::vector<RelativeIndex>>& search_result) const{
    std::pair<int, float> search_list;
    std::vector<std::pair<int, float>> pre_result;
    std::vector<std::vector<std::pair<int, float>>> result;
    

    for(int i = 0; i < search_result.size(); i++) {
        std::cout << "Request #" << i << std::endl;
        for(int t = 0; t < search_result[i].size(); t++) {
            if(search_result[i][t].rank == 0) {
                std::cout << "\t\t\tNothing was found in the documents!" << std::endl;
                search_list.first = 0;
                search_list.second = 0;
                pre_result.push_back(search_list);
                break;
            }
            else {
                std::cout << "\tDoc_ID " << search_result[i][t].doc_id << " \tRANK " << search_result[i][t].rank << std::endl; 
                search_list.first = search_result[i][t].doc_id;
                search_list.second = search_result[i][t].rank;
                pre_result.push_back(search_list);
            }
        }
        result.push_back(pre_result);
        pre_result.clear();
    }
    return result;
}