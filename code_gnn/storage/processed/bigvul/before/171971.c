static inline bool should_forward(tETH_HDR* hdr)
{
 uint16_t proto = ntohs(hdr->h_proto);
 if (proto == ETH_P_IP || proto == ETH_P_ARP || proto == ETH_P_IPV6)
 return true;
    BTIF_TRACE_DEBUG("unknown proto:%x", proto);
 return false;
}
