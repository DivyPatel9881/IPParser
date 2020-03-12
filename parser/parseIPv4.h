#ifndef IP_PARSER_PARSER_PARSE_IPV4
#define IP_PARSER_PARSER_PARSE_IPV4

#include <string>

#include "./IP.h"
#include "./dtoi.h"
#include "./byte.h"
#include "./IPv4.h"

using namespace std;

IP parseIPv4(string s) {
	// var p [IPv4len]byte
    vector <byte> pb;
    IP ipv4;
	for(int i = 0; i < IP :: IPv4len ; i++) {
		if(s.length() == 0) {
			// Missing octets.
            ipv4.isLegal = false;
			return ipv4;
		}
		if(i > 0) {
			if(s[0] != '.') {
                ipv4.isLegal = false;
				return ipv4;
			}
			s = s.substr(1,s.length() - 1);
		}
		pair<int,int> p = dtoi(s);
		if ((p.first>=big || p.second==0) || p.first > 0xFF) {
            ipv4.isLegal = false;
			return ipv4;
		}
        s = s.substr(p.second, s.length() - p.second);
		pb[i] = p.first;
	}
	if(s.length()!= 0) {
		ipv4.isLegal = false;
		return ipv4;
	}
	return IPv4(pb[0], pb[1], pb[2], pb[3]);
}

#endif