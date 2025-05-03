#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <thread>


#include "converterjson.h"
#include "invertedindex.h"
#include "searchserver.h"


int main() {
    ConverterJSON converter;
    InvertedIndex invertedIndex;
    
    
    std::string answer;
    try {
        std::cout << "\t\t\t\"The Search Engine-2000\" welcomes you\n\n\n";
        
        //Получим данные из файлов
        const auto documents = converter.GetTextDocument();
        const auto request_list = converter.GetRequests();
        const auto limit = converter.GetResponsesLimit();


        //Обработаем, создав карту слов с привязкой к документу
        //Используем несколько потоков 
        invertedIndex.SetNumberDocument(documents.size());
        std::vector<std::thread> threads;
        
        for(int i = 0; i < documents.size(); i++) {
                threads.emplace_back(&InvertedIndex::UpdateDocumentBase, &invertedIndex, documents[i], i);
            }
        for (auto& thread : threads) {
                thread.join();
            }
        
        
        
        //Подготовим ответы на запрос
        //С выводом в консоль и записью в файл
        SearchServer searchserver(invertedIndex);
        const auto prepared_query = searchserver.request_processing(request_list);
        searchserver.console_answer(prepared_query);
        const auto preparing_a = searchserver.search(prepared_query);
        
 
        
        //Запишем ответ на запрос в файл
        const auto ready_answer = searchserver.preparing_response(preparing_a, limit);
        converter.putAnswer(ready_answer);
    }



    catch (const MissingKey &x) {
        std::cerr << x.what() << std::endl;
    }
    catch (const NotOpenFile &x) {
        std::cerr << x.what() << std::endl;
    }
    
    
    return 0;
    
    
}