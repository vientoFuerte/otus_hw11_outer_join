// boost_visualcpp_template.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <ctime>
#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include "join_server.h"
#include "table.h"



int main(int argc, char* argv[])
{

    if (argc != 2) {
            std::cerr << "Usage: join_server <port>\n";
            return 1;
    
    }
    
    std::cout << "SERVER PROGRAM" << std::endl;
    try
    {
        g_port = static_cast<unsigned short>(std::stoi(argv[1]));     // порт  
         
        // Создаем acceptor с переданным портом ("слушает" входящие соединения)
        acceptor = std::make_unique<tcp::acceptor>(
          io_context, 
          tcp::endpoint(tcp::v4(), g_port)   //tcp::v4() — "все доступные сетевые интерфейсы" (аналог 0.0.0.0)
         );
         
         //Начало приема соединений
         BeginAcceptConnection();

         //Ожидание событий (новые соединения, данные от клиентов) и вызов callback-функций при наступлении событий
         io_context.run(); // блокируется до завершения всех асинхронных операций
              
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
