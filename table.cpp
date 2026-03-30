#include "table.h"
#include "thread_queue.h"
#include <sstream>
#include <string>
#include <chrono>
#include <atomic>
#include <thread>

namespace table {

std::map<int, std::string> tableA;
std::map<int, std::string> tableB;
  
//Распечатать содержимое таблицы
void printTable(const std::map<int, std::string>& table)
{
    for(const auto &pair: table)
    {
       std::cout<<pair.first<<": " <<pair.second<<std::endl;
    }
}
//Распечатать содержимое обеих таблиц
void printTables()
{
    std::cout << "-----------------" << std::endl;
    printTable(tableA);
    printTable(tableB);
}
  
std::string parseCommand(const std::string& input) {
    std::stringstream ss(input);
    std::string command;
    ss >> command; // Считываем первое слово (команду)

    DLOG("parseCommand: operation=" << input << std::endl;)

    if (input.empty()) return "ERR ...\n";
    
    if (command == "INSERT") {
        std::string table, name;
        int id;
        // Ожидаем: INSERT table_name id name
        if (ss >> table >> id >> name) {
            if (table == "A") {
                tableA[id] = name;
                return "OK\n";
            } 
            else if (table == "B") {
                tableB[id] = name;
                return "OK\n";
            }
            else {
                return "ERR Unknown table: " + table + "\n";
            }
            
        }
    } 
    else if (command == "TRUNCATE") {
        std::string table;
        // Ожидаем: TRUNCATE table_name
        if (ss >> table) {
            if (table == "A") {
                tableA.clear();
                return "OK\n";
            } else if (table == "B") {
                tableB.clear();     
                return "OK\n";  
            }
            else {
                return "ERR Unknown table: " + table + "\n";
            }
            std::cout << "Команда: TRUNCATE, Таблица: " << table << std::endl;
        }
    } 
    else if (command == "INTERSECTION") {
        std::ostringstream response;
        response << "OK\n";
        for(const auto &pair: tableA)
        {
            auto it = tableB.find(pair.first);
            if(it!= tableB.end())
            {
               std::cout<<pair.first<<": " <<pair.second<<it->second<<std::endl;
            }
        }
        
        std::cout << "Команда: INTERSECTION" << std::endl; 
        return response.str();
        
    } 
    else if (command == "SYMMETRIC_DIFFERENCE") {        
        std::ostringstream response;
        response << "OK\n"; 
        for(const auto &pair: tableA)
        {
            auto it = tableB.find(pair.first);
            if(it == tableB.end())
            {
               std::cout<<pair.first<<": " <<pair.second<<std::endl;
            }
        }
        for(const auto &pair: tableB)
        {
            auto it = tableA.find(pair.first);
            if(it == tableA.end())
            {
               std::cout<<pair.first<<": " <<pair.second<<std::endl;
            }
        }
        std::cout << "Команда: SYMMETRIC_DIFFERENCE" << std::endl;
        return response.str();
        
    } 
    else {
        std::cout << "Неизвестная команда: " << command << std::endl;
    }
    
    printTables();
}


void receive(const char* data, std::size_t size) {

    //  копируем в строку size символов из data
    std::string input(data, size);
    //чтобы работать как с входным потоком
    std::istringstream stream(input);
    std::string line;
    std::string response;
    
    while (std::getline(stream, line))  {
      response = parseCommand(line);
      std::cout << "Response: " << response;
    }
    
}

} // namespace table 



