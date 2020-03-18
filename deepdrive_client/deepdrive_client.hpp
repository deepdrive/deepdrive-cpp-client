#pragma once

#include "vendor/include/zmq.hpp"
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

    /**
     * Get Deepdrive observation after executing action
     *
     * @param steering -1 to 1 steering
     * @param throttle -1 to 1 throttle
     * @param brake 0 to 1 brake
     * @param handbrake 0 to 1 handbrake (not currently used)
     * @param has_control Whether you want to control the vehicle or not. If
     *  has_control == false then in-game NPC agent will take over and drive for
     *  you.
     * **/

    rapidjson::Document
    step(double steering, double throttle, double brake, double handbrake, bool has_control);

    /** Reset the sim **/
    rapidjson::Document reset();

    /** Close sim **/
    rapidjson::Document close();

    bool verbose = false;

protected:

private:
    zmq::context_t _context = zmq::context_t(1);
    zmq::socket_t _socket = zmq::socket_t(_context, ZMQ_PAIR);

    void start_sim();

    rapidjson::Document
    send(rapidjson::Value &method, rapidjson::Value &args,
         rapidjson::Value &kwargs, rapidjson::Document &req,
         rapidjson::MemoryPoolAllocator<> &alloc);

    static rapidjson::Value
    get_action(double steering, double throttle, double brake,
               double handbrake, bool has_control,
               rapidjson::MemoryPoolAllocator<> &alloc);


};

}  // namespace deepdrive