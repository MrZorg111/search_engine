#include "invertedindex.h"


#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>


std::map<std::string, std::vector<Entry>> InvertedIndex::update_doc(std::string text, int num_doc) {
    bool flag_add_count = false;
    std::istringstream stream_text(text);
    std::string constructor_doc;
    std::map<std::string, std::vector<Entry>> tempo_map;
    while(stream_text >> constructor_doc) {
        if(!tempo_map.count(constructor_doc)) {
            std::vector<Entry> add_word;
            _entry.doc_id = num_doc;
            _entry.count = 1;
            add_word.push_back(_entry);
            tempo_map.insert(std::pair<std::string, std::vector<Entry>> (constructor_doc, add_word));
        }
        else {
            auto it = freq_dictionary.find(constructor_doc);
            for(auto& its: it->second) {
                if(its.doc_id == num_doc) {
                    its.count++;
                    flag_add_count = true;
                }
            }
            if(!flag_add_count) {
                _entry.doc_id = num_doc;
                _entry.count = 1;
                it->second.push_back(_entry);
            }
        }
        flag_add_count = false;
        constructor_doc.clear();
    }

    return tempo_map;
}

//Составляем карту слов с привязкой к документу и кол-во раз появления в тексте
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_word) {
    for(int i = 0; i < input_word.size(); i++) {
        update_doc(input_word[i], i);
    }
}

//Производим поиск по карте полученного слова. 
//Выводим документ и кол-во раз появления в нем искомого слова
//В случае отсутствия слова в карте, устанавливаем обе позиции на 0
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    std::vector<Entry> words;

    if(freq_dictionary.count(word)) {
        auto it = freq_dictionary.find(word);
        
        for(int i = 0; i < it->second.size(); i++) {
            words.push_back(it->second[i]);
        }
    }
    else {
        Entry tempo;
        tempo.doc_id = 0;
        tempo.count = 0;
        words.push_back(tempo);
    }
    return words;
}

//Выводит общее кол-во документов для поиска
int InvertedIndex::GetNumberDocument() {
    return docs.size();
}