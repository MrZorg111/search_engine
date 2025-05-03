#include "invertedindex.h"


#include <vector>
#include <sstream>
#include <string>

//Составляем карту слов с привязкой к документу и кол-во раз появления в тексте
void InvertedIndex::UpdateDocumentBase(std::string input_word, int num_doc) {
    std::mutex mut;
    bool flag_add_count = false;
    std::istringstream stream_text(input_word);
    std::string constructor_doc;
    mut.lock();
    while(stream_text >> constructor_doc) {
        if(!freq_dictionary.count(constructor_doc)) {
            std::vector<Entry> add_word;
            _entry.doc_id = num_doc;
            _entry.count = 1;
            add_word.push_back(_entry);
            freq_dictionary.insert(std::pair<std::string, std::vector<Entry>> (constructor_doc, add_word));
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
    mut.unlock();
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


void InvertedIndex::SetNumberDocument(int num) {
    num_docs = num;
}

int InvertedIndex::GetNumberDocument() {
    return num_docs;
}