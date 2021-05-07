#pragma once
#include "Component.h"
#include "DataType.h"
#include <vector>
#include <Winsock2.h>

class Controller;
class Decoder;

class Router: public Component<H264> {
public:
    Router(Decoder*, Controller*);
    bool setPort(int);
    bool link(std::string, int, int);
protected:
    void start();
    bool wait();
    void process();
    void end();
private:
    Decoder* decoder;
    Controller* controller;
    std::atomic_bool living;
    SOCKET upStream, entry;
    std::vector<SOCKET> downStream;
    std::mutex vecMtx;
    void server();
};