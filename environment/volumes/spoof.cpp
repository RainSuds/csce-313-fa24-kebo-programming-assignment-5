#include "common.h"

// Helper function to compute checksum
unsigned short checksum(void *b, int len) {
    unsigned short *buf = (unsigned short *)b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }        
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
        
    sum = (sum >> 16) + (sum & 0xFFFF); // add high 16 to low 16
    sum += (sum >> 16); // add carry
    result = ~sum; // truncate to 16 bits

    return result;
}

int main()
{
    char buffer[PACKET_LEN];
    memset(buffer, 0, PACKET_LEN);

    ipheader *ip = (ipheader *)buffer;
    udpheader *udp = (udpheader *)(buffer + sizeof(ipheader));

    // add data
    char *data = (char *)udp + sizeof(udpheader);
    int data_len = strlen(CLIENT_IP);
    strncpy(data, CLIENT_IP, data_len);

    // create udp header
    // TODO
    udp->udp_sport = htons(CLIENT_PORT);
    udp->udp_dport = htons(SERVER_PORT);
    udp->udp_ulen = htons(sizeof(udpheader) + data_len);
    udp->udp_sum = 0;

    // create ip header
    // TODO
    ip->iph_ihl = 5; // Internet Header Length
    ip->iph_ver = 4; // IPv4
    ip->iph_ttl = 64; // Time to Live
    ip->iph_sourceip.s_addr = inet_addr(SPOOF_IP); // spoofed src
    ip->iph_destip.s_addr = inet_addr(SERVER_IP); // des
    ip->iph_protocol = IPPROTO_UDP; // UDP
    ip->iph_len = htons(sizeof(ipheader) + sizeof(udpheader) + data_len); // total length

    // calculate checksum
    ip->iph_chksum = checksum((unsigned short *)buffer, sizeof(ipheader) + sizeof(udpheader) + data_len);

    // send packet
    // TODO
    send_raw_ip_packet(ip);

    return 0;
}