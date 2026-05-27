#ifndef PACKETRULE_H
#define PACKETRULE_H

#include <string>
using namespace std;

class PacketRule {
public:
    int id;
    string sourceIp;
    string destinationIp;
    int priority;

    PacketRule(int id, string src, string dest, int prio) {
        this->id = id;
        sourceIp = src;
        destinationIp = dest;
        priority = prio;
    }
};

#endif