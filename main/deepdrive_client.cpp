#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

#include "deepdrive_client.hpp"

//  Receive 0MQ string from socket and convert into string
static std::string
s_recv (zmq::socket_t & socket) {

  zmq::message_t message;
  socket.recv(&message);

  return std::string(static_cast<char*>(message.data()), message.size());
}

void hello () {
  //  Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_PAIR);
  socket.connect("tcp://localhost:5555");

  std::cout << "Connected to ZMQ PAIR server at 0.0.0.0:5555" << std::endl;

  while (true) {
    zmq::message_t request;

    std::string string = s_recv (socket);
    std::cout << string << std::endl;

    //  Wait for next request from client
//    socket.recv (&request);
//    std::cout << "Received Hello" << std::endl;

    //  Do some 'work'
    sleep(1);

    //  Send reply back to client
    zmq::message_t reply (5);
    memcpy (reply.data (), "World", 5);
    socket.send (reply);
  }
}

namespace deepdrive {
  DeepdriveClient::DeepdriveClient() {

  }

  DDOut DeepdriveClient::step() {
    return {};
  }

  DeepdriveClient::~DeepdriveClient() = default;

}

