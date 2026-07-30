/* chdb WASI stub: no ARP on WASI. */
#ifndef CHDB_WASI_COMPAT_NET_IF_ARP_H
#define CHDB_WASI_COMPAT_NET_IF_ARP_H

#include <sys/socket.h>

#define ARPHRD_ETHER 1
#define ARPHRD_IEEE802 6
#define ARPHRD_DLCI 15
#define ARPHRD_ATM 19
#define ARPHRD_PPP 512
#define ARPHRD_TUNNEL 768
#define ARPHRD_TUNNEL6 769
#define ARPHRD_LOOPBACK 772
#define ARPHRD_IEEE1394 24
#define ARPHRD_IEEE80211 801

struct arpreq
{
    struct sockaddr arp_pa;
    struct sockaddr arp_ha;
    int arp_flags;
    struct sockaddr arp_netmask;
    char arp_dev[16];
};

#endif
