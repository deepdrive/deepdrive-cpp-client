#include <iostream>
#include <utility>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <zconf.h>

#include "deepdrive_client.hpp"

//  Receive 0MQ string from socket and convert into string
static std::string s_recv(zmq::socket_t &socket) {

    zmq::message_t message;
    socket.recv(&message);

    return std::string(static_cast<char *>(message.data()), message.size());
}

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
{
    _socket.connect("tcp://localhost:5557");
    std::cout << "Connected to ZMQ PAIR server at 0.0.0.0:5557" << std::endl;
    start_sim();
//    simple_call_resp_loop();

}

void DeepdriveClient::simple_call_resp_loop() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    // TODO: Remove while loop, just for testing
    while (true) {
        zmq::message_t msg(5);
        memcpy(msg.data(), "World", 5);
        _socket.send(msg);

        std::string string = s_recv(_socket);
        std::cout << string << std::endl;

        sleep(1);
    }
#pragma clang diagnostic pop
}

/**
 * The sim can be configured remotely or locally. To configure remotely,
 * just start the server process with --json-server. Then pass kwargs similar
 * to the following
 *
 * kwargs = {'is_remote_client': True, 'render': True, 'cameras': [
 *  {'name': 'My cam!', 'field_of_view': 60, 'capture_width': 227,
 *   'capture_height': 227, 'relative_position': [150, 1.0, 200],
 *   'relative_rotation': [0.0, 0.0, 0.0]}]}
 *
 */
void DeepdriveClient::start_sim() {
    rapidjson::Value args(rapidjson::kArrayType);
    rapidjson::Value kwargs(rapidjson::kObjectType);
    rapidjson::Value start("start");
    rapidjson::Document resp = send(start, args, kwargs);
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

    std::string resp_str = s_recv(_socket);
    rapidjson::Document res;
    res.Parse(resp_str.c_str());
    std::cout << res.GetString() << std::endl;

    return res;
}

/** Server methods **/
rapidjson::Document
DeepdriveClient::step(rapidjson::Value action) {
    rapidjson::Value args(rapidjson::kArrayType);
    rapidjson::Value method("step");
    return send(method, args, action);
}


rapidjson::Document DeepdriveClient::reset() {
    rapidjson::Value method("reset");
    rapidjson::Value args(rapidjson::kArrayType);
    rapidjson::Value kwargs(rapidjson::kObjectType);
    return send(method, args, kwargs);
};

rapidjson::Document
DeepdriveClient::close() {
    rapidjson::Value method("close");
    rapidjson::Value args(rapidjson::kArrayType);
    rapidjson::Value kwargs(rapidjson::kObjectType);
    return send(method, args, kwargs);
}

/** End server methods **/

DeepdriveClient::~DeepdriveClient() {
    close();
    _socket.close();
}

rapidjson::Value
DeepdriveClient::get_action(double steering, double throttle, double brake,
                            double handbrake, bool has_control) {
    rapidjson::Value action(rapidjson::kObjectType);
    {
        rapidjson::Value steering_key("steering");
        rapidjson::Value steering_value(steering);
        action.AddMember(steering_key, steering_value, _alloc);

        rapidjson::Value throttle_key("throttle");
        rapidjson::Value throttle_value(throttle);
        action.AddMember(throttle_key, throttle_value, _alloc);

        rapidjson::Value brake_key("brake");
        rapidjson::Value brake_value(brake);
        action.AddMember(brake_key, brake_value, _alloc);

        rapidjson::Value handbrake_key("handbrake");
        rapidjson::Value handbrake_value(handbrake);
        action.AddMember(handbrake_key, handbrake_value, _alloc);

        rapidjson::Value has_control_key("has_control");
        rapidjson::Value has_control_value(has_control);
        action.AddMember(has_control_key, has_control_value, _alloc);
    }
    return action;
}

} // end deepdrive namespace
