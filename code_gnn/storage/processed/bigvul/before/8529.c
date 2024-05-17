static void ssh1_pkt_addmp(struct Packet *pkt, Bignum b)
{
    int len = ssh1_bignum_length(b);
    unsigned char *data = snewn(len, unsigned char);
    (void) ssh1_write_bignum(data, b);
    ssh_pkt_adddata(pkt, data, len);
    sfree(data);
}
