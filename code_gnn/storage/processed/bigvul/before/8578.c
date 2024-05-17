static void *ssh_pkt_getdata(struct Packet *pkt, int length)
{
    if (pkt->length - pkt->savedpos < length)
	return NULL;
    pkt->savedpos += length;
    return pkt->body + (pkt->savedpos - length);
}
