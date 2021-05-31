// MIT License

// Copyright (c) 2021 YeiTingHao ShiChengFeng

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef STREAM_ROUTER_H_
#define STREAM_ROUTER_H_

#ifdef WIN32 // windows system marco
#include <Winsock2.h>
typedef int socklen_t;
#else // linux system marco
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define closesocket close
typedef int SOCKET;
#endif // WIN32

#include <vector>

#include "Component.h"
#include "Controller.h"
#include "DataType.h"

class Controller;
class Decoder;

// A class to receive streaming packet and send to downstream.
// Example:
// Router* rou = new Router(decoder, this);
// // start living
// if (rou->setPort(port)) {
//     rou->run();
// }
// // stop living
// rou->stop();
// // start watching
// if (rou->link(ip, port, outPort)) {
//     rou->run();
// }
// // stop watching
// rou->stop();
class Router: public Component<H264> {
public:
    // Constructors
    // Set decoder pointer and controller pointer
    Router(Decoder*, Controller*);
    // Set video streaming ontput port
    // Called before start living
    bool setPort(uint16_t);
    // Connent to streaming server and set streaming ontput port
    // Called before start watching
    bool link(std::string, int, int);
protected:
    // Override component function
    // Open a thread to listen connect
    void start();
    // Override component function
    // Return ture if is living and queue is enpty
    // If watching, called controller's leave function when server is disconnent
    bool wait();
    // Override component function
    // Send packet to each downstream node
    void process();
    // Override component function
    // Close socket and clear downstream and queue
    void end();
private:
    // Decoder component pointer
    Decoder* decoder;
    // Controller component pointer
    Controller* controller;
    // Living state
    std::atomic_bool living;
    // Connected server socket and listening connect socket
    SOCKET up_stream, entry;
    // Downstream connected socket list
    std::vector<SOCKET> down_stream;
    // Mutex lock for downstream list
    std::mutex vec_mtx;
    // The main function for Listening connect thread
    // Accpet connect and push socket in downstream list
    void server();
};

#endif // STREAM_ROUTER_H_
