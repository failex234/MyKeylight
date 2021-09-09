#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <arpa/inet.h>
#include "Keylight.h"
#include "thirdparty/zeroconf.hpp"

void* get_in_addr(sockaddr_storage* sa)
{
    if (sa->ss_family == AF_INET)
        return &reinterpret_cast<sockaddr_in*>(sa)->sin_addr;

    if (sa->ss_family == AF_INET6)
        return &reinterpret_cast<sockaddr_in6*>(sa)->sin6_addr;

    return nullptr;
}

bool discoverKeylights(std::vector<Keylight> *keylights) {
    std::vector<Zeroconf::mdns_responce> keylights_zeroconf;
    //Search for elgato keylights in the local network using mDNS (Keylights announce their service via _elg._tcp.local)
    bool st = Zeroconf::Resolve("_elg._tcp.local", /*scanTime*/ 3, &keylights_zeroconf);

    //This should never happen but should be accounted for
    if (!st) {
        std::cerr << "Unable to query to local network (MDNS query failed)" << std::endl;
        return false;
    }


    //Go through all dns records of a response to extract the contents of the SRV record
    for (auto item : keylights_zeroconf) {

        //Buffer for the ip address
        char buffer[INET6_ADDRSTRLEN + 1] = {0};
        inet_ntop(item.peer.ss_family, get_in_addr(&item.peer), buffer, INET6_ADDRSTRLEN);

        std::string name;

        if (!item.records.empty())
        {
            for (auto & rr : item.records)
            {
                switch(rr.type)
                {
                    //SRV record
                    case 33:
                        //The SRV record contains the given name of this particular keylight
                        name = std::string(rr.name);
                        break;
                }
            }
        }

        Keylight *ke = new Keylight(name, std::string(buffer));

        keylights->push_back(*ke);
    }
    return true;
}