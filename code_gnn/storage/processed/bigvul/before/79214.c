static void StreamTcpPseudoPacketSetupHeader(Packet *np, Packet *p)
{
     
    if (PKT_IS_IPV4(p)) {
        np->ip4h = (IPV4Hdr *)((uint8_t *)GET_PKT_DATA(np) + (GET_PKT_LEN(np) - IPV4_GET_IPLEN(p)));
        PSEUDO_PKT_SET_IPV4HDR(np->ip4h, p->ip4h);

         
        np->tcph = (TCPHdr *)((uint8_t *)np->ip4h + IPV4_GET_HLEN(np));

        PSEUDO_PKT_SET_TCPHDR(np->tcph, p->tcph);

         
        SET_IPV4_SRC_ADDR(p, &np->dst);
        SET_IPV4_DST_ADDR(p, &np->src);
        SET_TCP_SRC_PORT(p, &np->dp);
        SET_TCP_DST_PORT(p, &np->sp);

    } else if (PKT_IS_IPV6(p)) {
        np->ip6h = (IPV6Hdr *)((uint8_t *)GET_PKT_DATA(np) + (GET_PKT_LEN(np) - IPV6_GET_PLEN(p) - IPV6_HEADER_LEN));
        PSEUDO_PKT_SET_IPV6HDR(np->ip6h, p->ip6h);

         
        np->tcph = (TCPHdr *)((uint8_t *)np->ip6h + IPV6_HEADER_LEN);
        PSEUDO_PKT_SET_TCPHDR(np->tcph, p->tcph);

         
        SET_IPV6_SRC_ADDR(p, &np->dst);
        SET_IPV6_DST_ADDR(p, &np->src);
        SET_TCP_SRC_PORT(p, &np->dp);
        SET_TCP_DST_PORT(p, &np->sp);
    }

     
    np->payload = NULL;
    np->payload_len = 0;
}
