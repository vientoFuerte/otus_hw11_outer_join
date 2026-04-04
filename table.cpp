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
                // Проверяем существование ключа
                if (tableA.find(id) != tableA.end()) {
                    return "ERR duplicate " + std::to_string(id) + "\n";
                }
                else
                {
                    tableA[id] = name;
                    return "OK\n";
                }

            } 
            else if (table == "B") {
                // Проверяем существование ключа
                if (tableB.find(id) != tableB.end()) {
                    return "ERR duplicate " + std::to_string(id) + "\n";
                }
                else
                {
                    tableB[id] = name;
                    return "OK\n";       
                }

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
        }
        // обработка ошибки, если не удалось прочитать таблицу
        return "ERR TRUNCATE requires table name\n";
    } 
    else if (command == "INTERSECTION") {
        std::ostringstream response;
        
        for(const auto &pair: tableA)
        {
            auto it = tableB.find(pair.first);
            if(it!= tableB.end())
            {
              // Формат: id,valueA,valueB
              response << pair.first << "," << pair.second << "," << it->second << "\n";
            }
        }
        response << "OK\n";
        return response.str();
        
    } 
    else if (command == "SYMMETRIC_DIFFERENCE") {        
        std::ostringstream response;
        for(const auto &pair: tableA)
        {
            auto it = tableB.find(pair.first);
            if(it == tableB.end())
            {
                // Формат: id,valueA, (пустое значение для B)
                response << pair.first << "," << pair.second << ",\n";
            }
        }
        for(const auto &pair: tableB)
        {
            auto it = tableA.find(pair.first);
            if(it == tableA.end())
            {
               response << pair.first << ",," << pair.second << "\n";
            }
        }
        response << "OK\n"; 
        return response.str();
        
    } 
    else {
        return "ERR Unknown command: " + command + "\n";  // Возвращаем ошибку
    }
    
    return "ERR Internal error\n";
}


std::string receive(const std::string& data) {

    return parseCommand(data);
}

} // namespace table 



