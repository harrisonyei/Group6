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

#include "Router.h"

#include <iostream>

#include "Controller.h"
#include "Decoder.h"

Router::Router(Decoder* _decoder,Controller* _controller) {
    decoder = _decoder;
    controller = _controller;
    living = true;
#ifdef WIN32
    WSAData wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
        std::cerr << "WSAStartup failed.\n";
    }
#endif // WIN32
}

bool Router::setPort(uint16_t port) {
    entry = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (entry < 0) {
        std::cerr << "create socket failed.\n";
        return false;
    }
    sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (::bind(entry, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind Socket failed.\n";
        closesocket(entry);
        return false;
    }
    return true;
}

bool Router::link(std::string ip, int port,int outPort) {
    up_stream = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (up_stream < 0) {
        std::cerr << "create socket failed.\n";
        return false;
    }
    sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    if (connect(up_stream, (const sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "connect error.\n";
        closesocket(up_stream);
        return false;
    }
    living = false;
    return setPort(outPort);
}

void Router::start() {
    std::lock_guard<std::mutex> lock(vec_mtx);
    down_stream.clear();
    std::thread t(&Router::server, this);
    t.detach();
}

bool Router::wait() {
    if (living) {
        return getSize() == 0;
    }
    else {
        std::shared_ptr<H264> h264(new H264());
        if (recv(up_stream, (char*)&(h264->size), sizeof(h264->size), MSG_WAITALL)) {
            h264->data = new uint8_t[h264->size];
            if (recv(up_stream, (char*)h264->data, h264->size, MSG_WAITALL)) {
                pushData(h264);
            }
            else {
                controller->leave();
            }
        }
        else {
            controller->leave();
        }
        return false;
    }
}

void Router::process() {
    std::lock_guard<std::mutex> lock(vec_mtx);
    std::shared_ptr<H264> h264(new H264());
    if (getSize()) {
        h264 = getData();
        char* buffer;
        buffer = new char[sizeof(h264->size) + h264->size];
        memcpy(buffer, &(h264->size), sizeof(h264->size));
        memcpy(buffer + sizeof(h264->size), h264->data, h264->size);
        int i = 0, bytes;
        while (i < down_stream.size()) {
            bytes = send(down_stream[i], buffer, sizeof(h264->size) + h264->size, 0);
            if (bytes < 0) {
                down_stream.erase(down_stream.begin() + i);
                std::cout << "socket: " << down_stream[i] << " disconnect.\n";
            }
            else {
                i++;
            }
        }
        decoder->receive(h264);
    }
}

void Router::end() {
    std::lock_guard<std::mutex> lock(vec_mtx);
    living = true;
    closesocket(up_stream);
    closesocket(entry);
    for (int i = 0; i < down_stream.size(); i++) {
        closesocket(down_stream[i]);
    }
    down_stream.clear();
    while (getSize()) {
        getData();
    }
}

void Router::server() {
    sockaddr_in addr;
    SOCKET down;
    int addrlen = sizeof(addr);
    listen(entry, 128);
    while (isActive()) {
        down = accept(entry, (struct sockaddr*)&addr, (socklen_t*)&addrlen);
        vec_mtx.lock();
        down_stream.push_back(down);
        vec_mtx.unlock();
        std::cout << inet_ntoa(addr.sin_addr) << " connect." << std::endl;
    }
}
