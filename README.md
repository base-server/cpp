# base-server-cpp

## Features
 - socket server

## Supported Platforms
 - requires a compiler that conforms to the C++17 standard or higher.

### Operating Systems
 - CentOS

### Compiler
 - gcc 8.3.1

### Build System
 - cmake 3.12

## Build
 - basic
   - mkdir build && cd build
   - cmake ../ && make -j4 install
   - output
     - ./install/socket-server
 - test
   - mkdir build && cd build
   - cmake ../ -DENABLE_TEST=on && make -j4 install && make test
 - code coverage
   - dnf install perl-Digest-MD5
   - mkdir build && cd build
   - cmake ../ -DENABLE_TEST=on && make -j4 install && make test && make lcov_run
   - run ./coverage/html/index.html in your browser

## Operation
 - start
   - standalone
     - ./build/install/socket-server -c ./test_data/config/ -s
   - non standalone
     - ./build/install/socket-server -c ./test_data/config/
 - stop
```bash
$ telnet 0 10000
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
=== greeting ===
stop
200 ok
```
 - admin
```bash
$ telnet 0 10000
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
=== greeting ===
help
help : print command
log_reconfigure : log reconfigure
quit : connect end
stop : process stop
200 ok
quit
200 ok
```
 - user
```bash
$ telnet 0 10001
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
=== greeting ===
test1
[response] test1
test2
[response] test2
quit
```
 - log
   - /tmp/test/log/socket_server_YYYYMMDD.log
 - config
   - ./test_data/config/
 - modify the behavior
   - modify the job function in the Start function of UserSocketServerJob.cpp
