#include <zmq.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

int main(int argc, char * argv[]) {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:555" + std::string(argv[2])); // Connect to the publisher's address
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0); // Subscribe to all messages

    std::cout << "Subscriber is running..." << std::endl;

    // argv[1] payload, argv[2] frequency
    // argv[3] n_sub, argv[4] m_pub
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
    int64_t latency[500];
    int messageNumber = 0;
    while (messageNumber<505) {
        zmq::message_t message;
        subscriber.recv(&message);

        // Calculate the latency
        int64_t currentTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        
        int64_t sentTime;
        memcpy(&sentTime, message.data(), sizeof(int64_t));
        double latencyMs = static_cast<double>(currentTime - sentTime);
        if(messageNumber>=5) latency[messageNumber-5] = latencyMs;
        // std::cout << "Received: Latency: " << latencyMs << " us" << std::endl;

        // std::string receivedString(static_cast<char*>(message.data()) + sizeof(int64_t), 8);

        // std::cout << "string: " << receivedString << std::endl;

        int8_t receivedArray[messageSize];
        memcpy(receivedArray, static_cast<char*>(message.data()) + sizeof(int64_t) + 8, 8);
        // std::cout << "data: " << static_cast<int>(receivedArray[0]) << std::endl;
        messageNumber++;
    }

    // write to file
    std::cout << "finish! write to csv file" << std::endl;
    std::ofstream myfile;
    myfile.open("result/latency/tmp/" + std::string("latency_") + std::string("subscriber_") + std::string(argv[3]) + std::string(".csv"));
    for(int i=0; i<500; i++){
        myfile << latency[i] << ',';
    }
    myfile.close();

    return 0;
}
