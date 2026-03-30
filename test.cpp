
#define BOOST_TEST_MODULE JoinServerTests
#include <boost/test/included/unit_test.hpp>
#include <thread>
#include "table.h"
#include "join_server.h"


//тест вставки в таблицу и проверки дубликата
BOOST_AUTO_TEST_CASE(test_insert_and_duplicate) {

    std::string test1 = "INSERT A 1 apple\n";
    std::string test2 = "INSERT A 1 apple2\n";
    std::string test3 = "INSERT B 1 apple\n";

    // Очищаем таблицы перед тестом
    table::tableA.clear();
    table::tableB.clear();
    
    // Успешная вставка
    std::string response1 = table::receive(test1.c_str(), test1.size());
    BOOST_CHECK(response1 == "OK\n");
    
    // Вставка с дубликатом
    std::string response2 = table::receive(test2.c_str(), test2.size());
    BOOST_CHECK(response2 == "ERR duplicate 1\n");
    
    // Вставка в таблицу B
    std::string response3 = table::receive(test3.c_str(), test3.size());
    BOOST_CHECK(response3 == "OK\n");
}


//тест truncate
BOOST_AUTO_TEST_CASE(test_truncate) {

    std::string insert1 = "INSERT A 1 test1\n";
    std::string insert2 = "INSERT A 2 test2\n";
    std::string truncateA = "TRUNCATE A\n";
    std::string truncateB = "TRUNCATE B\n";
    
    table::tableA.clear();
    table::tableB.clear();
    
    // Добавляем данные
    table::receive(insert1.c_str(), insert1.size());
    table::receive(insert2.c_str(), insert2.size());

    // Проверяем что данные есть
    BOOST_CHECK(table::tableA.size() == 2);
    
    std::string response = table::receive(truncateA.c_str(), truncateA.size());
    BOOST_CHECK(response == "OK\n");
    BOOST_CHECK(table::tableA.empty());
}
