#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>

int main(int argc, char * argv[]) {
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    // argv[3] publisher no.
    publisher.bind("tcp://*:555" + std::string(argv[3])); // Bind to port 5555
    sleep(1);
    std::cout << "Publisher is running..." << std::endl;

    // argv[1] payload, argv[2] frequency
    
    size_t valueEnd = std::string(argv[1]).find_first_not_of("0123456789");
    std::string valueStr = std::string(argv[1]).substr(0, valueEnd);
    size_t value = stoi(valueStr);
    std::string unit = std::string(argv[1]).substr(valueEnd);
    size_t messageSize;
    if (unit == "KB" || unit == "kb") {
        messageSize = value * 1000;
    } else if (unit == "MB" || unit == "mb") {
        messageSize = value * 1000 * 1000;
    } else {
        messageSize = value;
    }

    int messageNumber = 0;
    while (messageNumber<505) {
        // Create an int8 array message
        int8_t* message = new int8_t[messageSize];
        message[0] = messageNumber;
        std::string frame_id = "frame000";

        // Get the current time in microseconds
        int64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        // Combine the timestamp and message
        zmq::message_t zmqMessage(sizeof(int64_t) + frame_id.size() + messageSize);
        memcpy(zmqMessage.data() , &timestamp, sizeof(int64_t));
        memcpy(zmqMessage.data() + sizeof(int64_t), frame_id.c_str(), frame_id.size());
        memcpy(static_cast<char*>(zmqMessage.data()) + sizeof(int64_t) + frame_id.size(), message, messageSize);

        // Send the message
        publisher.send(zmqMessage);
        delete[] message;
        messageNumber++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/std::stoi(argv[2]))); // Delay for demonstration purposes
    }

    return 0;
}
