#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h> // For sleep function
#include <chrono>

int main() {
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5555"); // Bind to port 5555

    std::cout << "Publisher is running..." << std::endl;

    int messageNumber = 0;
    while (true) {
        unsigned long ms = std::chrono::system_clock::now().time_since_epoch() /
        std::chrono::milliseconds(1);
        
        zmq::message_t message(20);
        snprintf((char *)message.data(), 20, "Message #%d", messageNumber);
        publisher.send(message);
        std::cout << "Published: " << (char *)message.data() << std::endl;
        messageNumber++;
        sleep(1); // Delay for demonstration purposes
    }

    return 0;
}