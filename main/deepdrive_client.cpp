#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

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
    : _context(1)
    , _socket(_context, ZMQ_PAIR)
{
    rapidjson::Document d;
    d.Parse("{}");

    // 2. Modify it by DOM.
//    rapidjson::Value& s = d["stars"];
//    s.SetInt(s.GetInt() + 1);

    ;
    rapidjson::MemoryPoolAllocator<> &alloc = d.GetAllocator();
    d.AddMember("method", rapidjson::Value ("start"), alloc);
    d.AddMember("args", rapidjson::Value (rapidjson::kArrayType), alloc);
    d.AddMember("kwargs", rapidjson::Value (rapidjson::kObjectType), alloc);

    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    std::cout << buffer.GetString() << std::endl;


    //  Prepare our context and socket
    _socket.connect("tcp://localhost:5557");
    std::cout << "Connected to ZMQ PAIR server at 0.0.0.0:5557" << std::endl;

    //  Start server
    zmq::message_t start_request(buffer.GetSize());
    memcpy(start_request.data(), buffer.GetString(), buffer.GetSize());
    _socket.send(start_request);

    std::cout << buffer.GetString() << std::endl;

// TODO: Remove while loop, just for testing
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        zmq::message_t request;

        std::string string = s_recv(_socket);
        std::cout << string << std::endl;

        zmq::message_t req(buffer.GetSize());
        memcpy(req.data(), buffer.GetString(), buffer.GetSize());
        _socket.send(req);

        sleep(1);
    }
#pragma clang diagnostic pop

}

DDOut DeepdriveClient::step() {
    return {};
}

DeepdriveClient::~DeepdriveClient() {
    _socket.close();
};

}

