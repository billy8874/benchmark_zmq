#!/bin/bash

g++ src/zmq_subscriber.cpp -o subtime -lzmq
g++ src/zmq_publisher.cpp -o pubtime -lzmq
g++ src/sys_stats.cpp -o sys_stats