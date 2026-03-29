
#define BOOST_TEST_MODULE AcyncServerTests
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <thread>
#include "table.h"
#include "join_server.h"


//тест для статического блока (без фигурных скобок)
BOOST_AUTO_TEST_CASE(static_blocks) {

    boost::test_tools::output_test_stream output;
    std::streambuf* old_cout;
    
    // Перехватываем std::cout
    old_cout = std::cout.rdbuf(output.rdbuf());
    
    
    size_t block_size = 3;
    const char* data = "1\n2\n3\n";
    const char* expected = "\nbulk : 1, 2, 3";
  


    // Восстанавливаем cout
    std::cout.rdbuf(old_cout);
    

}

//тест для динамического блока (с фигурными скобками)
BOOST_AUTO_TEST_CASE(dynamic_blocks) {

    boost::test_tools::output_test_stream output;
    std::streambuf* old_cout;
    
    // Перехватываем std::cout
    old_cout = std::cout.rdbuf(output.rdbuf());
  
    




    // Проверяем консольный вывод
    //BOOST_CHECK(output.is_equal(expected));

    // Восстанавливаем cout
    std::cout.rdbuf(old_cout);
    
}
