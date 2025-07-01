#include "invertedindex.h"

#include <vector>
#include <sstream>
#include <string>

void InvertedIndex::StartThread(const std::vector<std::string>& documents) {
    std::vector<std::thread> threads;
    threads.reserve(documents.size());
    for(int i = 0; i < documents.size(); i++) {
                threads.emplace_back(&InvertedIndex::UpdateDocumentBase, this, documents[i], i);
    }
    for (auto& thread : threads) {
            if(thread.joinable()) {
                thread.join();
            }
        }
}

void InvertedIndex::UpdateDocumentBase(std::string input_docs, int num_doc) {
    docs.push_back(input_docs);
    bool flag = true;
    std::string word;
    std::vector<Entry> tempo_struct;
    std::stringstream doc_data(input_docs);
    static std::mutex mut;
    std::lock_guard<std::mutex> lock(mut);
    
    while(doc_data >> word) {
        if(!freq_dictionary.count(word)) {
            _entry.doc_id = num_doc;
            _entry.count = 1;
            tempo_struct.push_back(_entry);
            freq_dictionary.insert(std::pair<std::string, std::vector<Entry>> (word, tempo_struct));
            tempo_struct.clear();
        }
        else {
            auto& tempo_list = *freq_dictionary.find(word);

            for(int t = 0; t < tempo_list.second.size(); t++) {
                if(tempo_list.second[t].doc_id == num_doc) {
                    tempo_list.second[t].count++;
                    flag = false;
                    break;
                }
            }
            
            if(flag) {
            _entry.doc_id = num_doc;
            _entry.count = 1;
            tempo_list.second.push_back(_entry);
            }
            flag = true;
                
        }
    }
    
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const{
    std::vector<Entry> words;

    if(freq_dictionary.count(word)) {
        auto it = freq_dictionary.find(word);
        

        for(int i = 0; i < it->second.size(); i++) {
            words.push_back(it->second[i]);
        }
        
        
    }
    else {
        Entry tempo;
        words.push_back(tempo);
    }
    
    for(int s = 0; s < words.size(); s++) {
        for(int t = s + 1; t < words.size(); t++) {
            if(words[s].doc_id > words[t].doc_id) {
                auto tempo = words[s];
                words[s] = words[t];
                words[t] = tempo;
            }
        }    
    }

    return words;
}

void InvertedIndex::ClearFreqDictionary(){
    freq_dictionary.clear();
}
