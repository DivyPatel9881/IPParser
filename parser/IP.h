#ifndef IP_PARSER_PARSER_IP
#define IP_PARSER_PARSER_IP

#include <vector>
#include <sstream>

#include "./allFF.h"
#include "./IPMask.h"
#include "./equal.h"
#include "./byte.h"
// #include "./isZeros.h"

using namespace std;

class IP {
    public:
        vector <byte> ip;
        bool isLegal;
        static byte IPv4len;
        static byte IPv6len;
        static IPMask v4InV6Prefix;

        IP() {
            isLegal = true;
        }

        IP Mask(IPMask mask) {
            IPMask mask_2(mask.begin(), mask.begin()+12);
            if(mask.size() == IPv6len && ip.size() == IPv4len && allFF(mask_2)) {
                IPMask mask_3(mask.begin() + 12, mask.end());
                mask = mask_3;
            }
            IPMask ip_2(ip.begin(), ip.begin() + 12);
            if(mask.size() == IPv4len && ip.size() == IPv6len && equal(ip_2, v4InV6Prefix)) {
                IPMask ip_3(ip.begin() + 12, ip.end());
                ip = ip_3;
            }
            unsigned int n = ip.size();
            if(n != mask.size()) {
                IP ip_mask;
                ip_mask.isLegal = false;
                return ip_mask;
            }
            IP out;
            vector <byte> outNew(n, 0);
            out.ip = outNew;
            out.isLegal = true;
            for(int i = 0; i < n; i++) {
                out.ip[i] = ip[i] & mask[i];
            }
            return out;
        }

        bool Equal(IP x) {
            if(ip.size() == x.ip.size()) {
                return equal(ip, x.ip);
            }
            if(ip.size() == IPv4len && x.ip.size() == IPv6len) {
                vector <byte> xNew1(x.ip.begin(), x.ip.begin() + 12);
                vector <byte> xNew2(x.ip.begin() + 12, x.ip.end());
                return equal(xNew1, v4InV6Prefix) && equal(ip, xNew2);
            }
            if(ip.size() == IPv6len && x.ip.size() == IPv4len) {
                vector <byte> ipNew1(ip.begin(), ip.begin() + 12);
                vector <byte> ipNew2(ip.begin() + 12, ip.end());
                return equal(ipNew1, v4InV6Prefix) && equal(ipNew2, x.ip);
            }
            return false;
        }

        string toString() {
            string ip1, ip2, ip3, ip4;
            stringstream ss1, ss2, ss3, ss4;
            ss1 << ip[12];
            ss1 >> ip1;
            ss2 << ip[13];
            ss2 >> ip2;
            ss3 << ip[14];
            ss3 >> ip3;
            ss4 << ip[15];
            ss4 >> ip4;
            return ip1 + "." + ip2 + "." + ip3 + "." + ip4;
        }

        // To4 converts the IPv4 address ip to a 4-byte representation.
        // If ip is not an IPv4 address, To4 returns nil.
        IP To4() {
            if(ip.size() == IPv4len) {
                return *this;
            }
            IP ipNew;
            vector <byte> ipN (ip.begin(), ip.begin() + 12);
            ipNew.ip = ipN;
            if(ip.size() == IPv6len && isZeros(ipNew) && ip[10] == 0xff && ip[11] == 0xff) {
                IP ipNew2;
                vector <byte> ipN(ip.begin() + 12, ip.begin() + 16);
                ipNew2.ip = ipN;
                return ipNew2;
            }
            this->isLegal = false;
            return *this;
        }

        // Is p all zeros?
        static bool isZeros(IP p) {
            for(int i = 0 ; i < p.ip.size() ; i++ ) {
                if(p.ip[i] != 0) {
                    return false;
                }
            }
            return true;
        }

};

byte IP :: IPv4len = 4;
byte IP :: IPv6len = 16;
IPMask IP :: v4InV6Prefix{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff};

#endif