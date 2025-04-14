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
    

    std::string answer;
    try {
        std::cout << "\t\t\t\"The Search Engine-2000\" welcomes you\n\n\n";

        invertedIndex.UpdateDocumentBase(converter.GetTextDocument());
        auto request_list = converter.GetRequests();
        
        SearchServer searchserver(invertedIndex);
 
        auto prepared_query = searchserver.request_processing(request_list);
        searchserver.console_answer(prepared_query);
        const auto preparing_a = searchserver.search(prepared_query);
        const auto limit = converter.GetResponsesLimit();
        auto ready_answer = searchserver.preparing_response(preparing_a, limit);
        converter.putAnswer(ready_answer);
    }
    catch (const MissingKey &x) {
        std::cerr << x.what() << std::endl;
    }
    catch (const NotOpenFile &x) {
        std::cerr << x.what() << std::endl;
    }
    
    
    system("pause");
    return 0;
    
    
}