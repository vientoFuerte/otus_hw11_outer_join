#include "join_server.h"
#include <iostream>
#include <sstream>
#include <string>

// Глобальные переменные (определения)
boost::asio::io_context io_context;
std::unique_ptr<tcp::acceptor> acceptor;
unsigned short g_port; 


std::shared_ptr<Connection> Connection::Create(boost::asio::io_context& io_context)
{
    return std::make_shared<Connection>(io_context);
}

void Connection::start_read() 
{
    auto self = shared_from_this();
    boost::asio::async_read_until(socket, streambuf_, '\n',
        [this, self](boost::system::error_code ec, std::size_t /*bytes*/) {
            if (!ec) {
                std::istream is(&streambuf_);
                std::string line;
                while(std::getline(is, line))
                {
                    std::string response = table::receive(line);
                    
                    // Копируем response в shared_ptr для безопасности
                    // иначе response в async_write может быть уничтожена до завершения записи
                    auto response_ptr = std::make_shared<std::string>(std::move(response));
                    
                    boost::asio::async_write(socket, 
                     boost::asio::buffer(*response_ptr),
                     [](boost::system::error_code ec, std::size_t) {
                         if (ec) {
                             // Ошибка записи - ничего не делаем
                         }
                     });
                    
                    
                }
                start_read();
            } else {
                if (streambuf_.size() > 0) {
                    std::cout << "=== BUFFER CONTENTS (" << streambuf_.size() << " bytes) ===" << std::endl;
                    std::string remaining(boost::asio::buffers_begin(streambuf_.data()),
                                          boost::asio::buffers_begin(streambuf_.data()) + streambuf_.size());
                    streambuf_.consume(streambuf_.size());
                    
                    std::istringstream is(remaining);
                    std::string line;
                    while(std::getline(is, line))
                    {
                        std::string response = table::receive(line);
                        
                        // Копируем response в shared_ptr для безопасности
                        // иначе response в async_write может быть уничтожена до завершения записи
                        auto response_ptr = std::make_shared<std::string>(std::move(response));
                        
                        boost::asio::async_write(socket, 
                         boost::asio::buffer(*response_ptr),
                         [](boost::system::error_code ec, std::size_t) {
                             if (ec) {
                                 // Ошибка записи - ничего не делаем
                             }
                         });                   
                    }
                }
                DLOG("Connection error, calling disconnect for ctx=" << ctx_ << std::endl);
            }
        });
}

// Реализация функций
void BeginAcceptConnection()
{
    std::shared_ptr<Connection> c = Connection::Create(io_context);
    acceptor->async_accept(c->socket,
        [c](const boost::system::error_code& err) {
            HandleAccept(c, err);
        });   
}

void HandleAccept(std::shared_ptr<Connection> connection, const boost::system::error_code& err)
{
    if (err) {
        //std::cout << err.message() << std::endl;
        BeginAcceptConnection();
    }
    else {
        ProcessConnection(connection);
        BeginAcceptConnection();
    } 
}

void ProcessConnection(std::shared_ptr<Connection> c)
{
    c->start_read();
}

