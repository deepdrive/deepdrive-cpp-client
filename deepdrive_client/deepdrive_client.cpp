#include <iostream>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "deepdrive_client.hpp"

//  Receive 0MQ string from socket and convert into string
//static std::string s_recv(zmq::socket_t &socket) {
//
//    zmq::message_t message;
//    socket.recv(&message);
//
//    return std::string(static_cast<char *>(message.data()), message.size());
//}

namespace deepdrive {
/*
 * A Client object acts as a remote proxy to the deepdrive gym environment.
 * Methods that you would call on the env, like step() are also called on
 * this object, with communication over the network -
 * rather than over shared memory (for observations) and network
 * (for transactions like reset) as is the case with the locally run
 * sim/gym_env.py.
 *
 * This allows the agent and environment to run on separate machines, but
 * with the same API as a local agent, namely the gym API.
 * The local gym environment is then run by api/server.py which proxies
 * RPC's from this client to the local environment.
 * All network communication happens over ZMQ to take advantage of their
 * highly optimized cross-language / cross-OS sockets.
 * NOTE: This will obviously run more slowly than a local agent which
 * communicates sensor data over shared memory.
*/

DeepdriveClient::DeepdriveClient()
    : _context(1)
    , _socket(_context, ZMQ_PAIR)
{
    _socket.connect("tcp://localhost:5557");
    std::cout << "Connected to ZMQ PAIR server at 0.0.0.0:5557" << std::endl;

    send_start_message();

    // 2. Modify it by DOM.
//    rapidjson::Value& s = d["stars"];
//    s.SetInt(s.GetInt() + 1);



        //// TODO: Remove while loop, just for testing
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wmissing-noreturn"
//    while (true) {
//        zmq::message_t request;
//
//        std::string string = s_recv(_socket);
//        std::cout << string << std::endl;
//
//        zmq::message_t req(buffer.GetSize());
//        memcpy(req.data(), buffer.GetString(), buffer.GetSize());
//        _socket.send(req);
//
//        sleep(1);
//    }
//#pragma clang diagnostic pop

}

void DeepdriveClient::send_start_message() {
    rapidjson::Value method("start");;
    rapidjson::Document d;
    rapidjson::MemoryPoolAllocator<> &alloc = d.GetAllocator();
    rapidjson::Value args(rapidjson::kArrayType);
    for (int i = 5; i <= 10; i++) {
        args.PushBack(i, alloc);
    }
    args.PushBack("Lua", alloc).PushBack("Mio", alloc);

    rapidjson::Value kwargs(rapidjson::kObjectType);
    {
        rapidjson::Value key("key");
        rapidjson::Value value("value");
        // adding elements to contacts array.
        kwargs.AddMember(key, value, alloc);
    }

    rapidjson::Document buffer = send(method, args, kwargs);

    std::cout << buffer.GetString() << std::endl;
}

rapidjson::Document
DeepdriveClient::send(rapidjson::Value &method, rapidjson::Value &args, rapidjson::Value &kwargs) {
    rapidjson::Document req;
    rapidjson::MemoryPoolAllocator<> &alloc = req.GetAllocator();
    req.Parse("{}");
    req.AddMember("method", method, alloc);
    req.AddMember("args", args, alloc);
    req.AddMember("kwargs", kwargs, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    req.Accept(writer);

    std::cout << buffer.GetString() << std::endl;

    zmq::message_t start_request(buffer.GetSize());
    memcpy(start_request.data(), buffer.GetString(), buffer.GetSize());
    _socket.send(start_request);

    rapidjson::Document res;
    res.Parse(buffer.GetString());

    return res;
}

rapidjson::Value
DeepdriveClient::step(rapidjson::Value action) {
    // TODO: Use move semantics to call this
    
    return {};
}

DeepdriveClient::~DeepdriveClient() {
    _socket.close();
};

}

