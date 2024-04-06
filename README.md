# Base Server for C++

## Features
 - socket server

<br/>

## Supported Platforms
 - requires a compiler that conforms to the C++20 standard or higher.

### Operating Systems
 - CentOS Stream release 9
 - Ubuntu 22.04.3 LTS

### Compiler
 - gcc version 13.1.0

### Build System
 - cmake version 3.20.2

<br/>

## Build
 - basic
   - `mkdir build && cd build`
   - `cmake ../ && make -j4 install`
   - output
     - `./install/socket-server`
 - test
   - `mkdir build && cd build`
   - `cmake ../ -DENABLE_TEST=on && make -j4 install && make test`
 - code coverage
   - prepare
     - CentOS Stream release 9
       - `dnf install perl-FindBin`
       - `dnf install perl-Time-HiRes`
       - `dnf install perl-CPAN`
       - `perl -MCPAN -e 'install Capture::Tiny'`
       - `perl -MCPAN -e 'install DateTime'`
       - `perl -MCPAN -e 'install Date::Parse'`
     - Ubuntu 22.04.3 LTS
       - `sudo perl -MCPAN -e 'install Capture::Tiny'`
       - `sudo perl -MCPAN -e 'install DateTime'`
   - `mkdir build && cd build`
   - `cmake ../ -DENABLE_TEST=on && make -j4 install && make test && make lcov_run`
   - run `./coverage/html/index.html` in your browser

<br/>

## Operation
### socket-server
 - start
   - standalone
     - `./build/install/socket-server -c ./test_data/config/ -s`
   - non standalone
     - `./build/install/socket-server -c ./test_data/config/`
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
   quit : connection end
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
   200 ok
   ```
 - log
   - `/tmp/test/log/socket_server_YYYYMMDD.log`
 - config
   - `./test_data/config/`
 - modify the user behavior
   - modify the job function of UserChildProcess.cpp
