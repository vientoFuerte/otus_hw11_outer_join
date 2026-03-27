// boost_visualcpp_template.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include "join_server.h"


int main(int argc, char* argv[])
{

    if (argc != 2) {
            std::cerr << "Usage: join_server <port>\n";
            return 1;
    
    }
    
    //std::cout << "SERVER PROGRAM" << std::endl;
    try
    {
        g_port = static_cast<unsigned short>(std::stoi(argv[1]));     // порт     
        // Запускаем потоки вывода один раз
        async::threads_start();
        
        // Создаем acceptor с переданным портом
        acceptor = std::make_unique<tcp::acceptor>(
          io_context, 
          tcp::endpoint(tcp::v4(), g_port)
         );
         BeginAcceptConnection();

         io_context.run(); // блокируется до завершения
      
         
         // После остановки io_context останавливаем потоки
         async::threads_stop();
              
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

