# ZeroMQ Benchmark [![cppzmq 4.10.0](https://img.shields.io/badge/cppzmq-4.10.0-blue)](https://github.com/zeromq/cppzmq)
This is a ZeroMQ package using cppzmq library to benchmarking and comparing the performance between different middlewares.

This package includes two parts:

* **Cppzmq package:** Follow tutorials on the ZeroMQ website and github repo of cppzmq to create publishers and subscribers. [cppzmq-4.10.0](https://github.com/zeromq/cppzmq)

* **Cpp Linux System Stats:** Use a github repo to help us get system status including CPU and Memory usage. [cpp-linux-system-stats](https://github.com/improvess/cpp-linux-system-stats)

## Building and Complie

First, follow instructions of [cppzmq-4.10.0](https://github.com/zeromq/cppzmq) repo to build zmq on computer.

Then, go to the root folder of the workspace using `./compile` to build.
```
./compile
```

Also, compile the system status cpp to get system information.
```
./compile_sys_stats_cpp.sh
```


## Launch and Execute
After building all necessary packages, run the following command to start a benchmark.
```
./run_pubsub.sh
```
You can modify m, n, hz, payload in benchmark.sh file to run specific combinations of the benchmark.
