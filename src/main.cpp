#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

#include "converterjson.h"
#include "invertedindex.h"
#include "searchserver.h"


int main() {
    ConverterJSON converter;
    InvertedIndex invertedIndex;
    
    try {
        converter.CheckConfig();
        std::cout << "\t\t\t\"The Search Engine-2000\" welcomes you\n\n\n";
        
        //Получим данные из файлов
        const auto documents = converter.GetTextDocument();
        const auto request_list = converter.GetRequests();


        
        //Обработаем, создав карту слов с привязкой к документу
        //Используем несколько потоков 
        invertedIndex.StartThread(documents);
        
        //Подготовим ответы на запрос
        //С выводом в консоль и записью в файл
        SearchServer searchserver(invertedIndex);
        const auto tempo_name = searchserver.PreRequest(request_list);
        const auto preparing_a = searchserver.Search(tempo_name);
        
 
        
        //Запишем ответ на запрос в файл
        const auto ready_answer = searchserver.PreAnswer(preparing_a);
        converter.PutAnswer(ready_answer);
    }

    
    


    catch (const MissingKey &x) {
        std::cerr << x.what() << std::endl;
    }
    catch (const NotOpenFile &x) {
        std::cerr << x.what() << std::endl;
    }

    return 0;
}