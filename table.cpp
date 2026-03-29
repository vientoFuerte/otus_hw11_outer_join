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
  
  
void parseCommand(const std::string& input) {
    std::stringstream ss(input);
    std::string command;
    ss >> command; // Считываем первое слово (команду)

    DLOG("parseCommand: operation=" << input << std::endl;)

    if (input.empty()) return;
    
    if (command == "INSERT") {
        std::string table, name;
        int id;
        // Ожидаем: INSERT table_name id name
        if (ss >> table >> id >> name) {
            std::cout << "Команда: INSERT, Таблица: " << table 
                      << ", ID: " << id << ", Имя: " << name << std::endl;
        }
    } 
    else if (command == "TRUNCATE") {
        std::string table;
        // Ожидаем: TRUNCATE table_name
        if (ss >> table) {
            std::cout << "Команда: TRUNCATE, Таблица: " << table << std::endl;
        }
    } 
    else if (command == "INTERSECTION") {
        std::cout << "Команда: INTERSECTION" << std::endl;
    } 
    else if (command == "SYMMETRIC_DIFFERENCE") {
        std::cout << "Команда: SYMMETRIC_DIFFERENCE" << std::endl;
    } 
    else {
        std::cout << "Неизвестная команда: " << command << std::endl;
    }
}


void receive(const char* data, std::size_t size) {

    //  копируем в строку size символов из data
    std::string input(data, size);
    //чтобы работать как с входным потоком
    std::istringstream stream(input);
    std::string line;
    
    while (std::getline(stream, line))  {
      parseCommand(line);
    }
    
}

} // namespace table 



