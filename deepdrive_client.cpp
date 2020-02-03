#include "deepdrive_client.h"

#include <iostream>

#include "zmqpp/context.hpp"
#include "zmqpp/message.hpp"
#include "zmqpp/loop.hpp"
#include "zmqpp/socket.hpp"

void hello() {
    std::cout << "Deepdrive client lib" << std::endl;
    zmqpp::context context;

    zmqpp::socket output(context, zmqpp::socket_type::pair);
    output.bind("inproc://test");
    zmqpp::socket input(context, zmqpp::socket_type::pair);
    input.connect("inproc://test");

    zmqpp::loop loop;

    bool socket_called = false;

    loop.add(output, [&socket_called]() -> bool { socket_called = true; return false; });
    loop.add(std::chrono::milliseconds(0), 1, [&loop, &output]() -> bool {
        loop.remove(output);
        loop.add(std::chrono::milliseconds(10), 1, []() -> bool { return false; });
        return true;
    });

    input.send("PING");

    loop.start();
    if(socket_called == false){
      std::cout << "ZMQ loop test successful" << std::endl;
    }
}

deepdrive::DeepdriveClient::DeepdriveClient() {

}

deepdrive::DeepdriveClient::~DeepdriveClient() = default;
