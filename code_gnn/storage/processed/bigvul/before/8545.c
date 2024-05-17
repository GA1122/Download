static Bignum ssh2_pkt_getmp(struct Packet *pkt)
{
    char *p;
    int length;
    Bignum b;

    ssh_pkt_getstring(pkt, &p, &length);
    if (!p)
	return NULL;
    if (p[0] & 0x80)
	return NULL;
    b = bignum_from_bytes((unsigned char *)p, length);
    return b;
}
