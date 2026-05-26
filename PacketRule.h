#ifndef PACKETRULE_H
#define PACKETRULE_H

#include <string>

class PacketRule {
public:
    int id;
    std::string sourceIp;
    std::string destinationIp;
    int priority;

    PacketRule(int id, std::string src, std::string dest, int prio)
        : id(id), sourceIp(src), destinationIp(dest), priority(prio) {}
};

#endif
