#include <zmq.hpp>
#include <string>
#include <iostream>

int main() {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555"); // Connect to the publisher's address
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0); // Subscribe to all messages

    std::cout << "Subscriber is running..." << std::endl;

    while (true) {
        zmq::message_t message;
        subscriber.recv(&message);
        std::cout << "Received: " << (char *)message.data() << std::endl;
    }

    return 0;
}