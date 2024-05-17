static void ssh_pkt_addstring_data(struct Packet *pkt, const char *data,
                                   int len)
{
    ssh_pkt_adddata(pkt, data, len);
    PUT_32BIT(pkt->data + pkt->savedpos - 4, pkt->length - pkt->savedpos);
}
