#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include "debug_settings.h"
#include "table.h"

using boost::asio::ip::tcp;

extern boost::asio::io_context io_context;
extern std::unique_ptr<tcp::acceptor> acceptor;
extern unsigned short g_port; 


// Класс, представляющий одно подключение
struct Connection : public std::enable_shared_from_this<Connection>
{
   tcp::socket socket;
   boost::asio::streambuf streambuf_;
   
   Connection(boost::asio::io_context& io_context) : socket(io_context) {}

   void start_read();
   
   static std::shared_ptr<Connection> Create(boost::asio::io_context& io_context);
   
};

// Функции
void BeginAcceptConnection();
void HandleAccept(std::shared_ptr<Connection> connection, const boost::system::error_code& err);
void ProcessConnection(std::shared_ptr<Connection> c);
