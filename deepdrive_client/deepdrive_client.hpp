#pragma once

#include <zmq.hpp>
#include <rapidjson/document.h>

namespace deepdrive {
/**
* ZMQ client for the Deepdrive simulator
*/
class DeepdriveClient {
public:
    /**
     * Contructs a new client.
     *
     */
    explicit DeepdriveClient(bool verbose=false);

    virtual ~DeepdriveClient();

    /** Get Deepdrive observation after executing action **/
    rapidjson::Document step(rapidjson::Value action);

    /** Reset the sim **/
    rapidjson::Document reset();

    /** Close sim **/
    rapidjson::Document close();

    /** Get server compatible action **/
    rapidjson::Value
    get_action(double steering, double throttle, double brake, double handbrake, bool has_control);

    bool verbose = false;

protected:

private:
    zmq::context_t _context = zmq::context_t(1);
    zmq::socket_t _socket = zmq::socket_t(_context, ZMQ_PAIR);
    rapidjson::MemoryPoolAllocator<> &_alloc = rapidjson::Document().GetAllocator();

    void start_sim();

    rapidjson::Document
    send(rapidjson::Value &method, rapidjson::Value &args, rapidjson::Value &kwargs);
};

}  // namespace deepdrive