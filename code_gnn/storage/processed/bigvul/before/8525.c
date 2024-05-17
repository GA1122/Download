static int s_wrpkt_prepare(Ssh ssh, struct Packet *pkt, int *offset_p)
{
    int pad, biglen, i, pktoffs;
    unsigned long crc;
#ifdef __SC__
     
    volatile
#endif
    int len;

    if (ssh->logctx)
        ssh1_log_outgoing_packet(ssh, pkt);

    if (ssh->v1_compressing) {
	unsigned char *compblk;
	int complen;
	zlib_compress_block(ssh->cs_comp_ctx,
			    pkt->data + 12, pkt->length - 12,
			    &compblk, &complen);
	ssh_pkt_ensure(pkt, complen + 2);    
	memcpy(pkt->data + 12, compblk, complen);
	sfree(compblk);
	pkt->length = complen + 12;
    }

    ssh_pkt_ensure(pkt, pkt->length + 4);  
    pkt->length += 4;
    len = pkt->length - 4 - 8;	 
    pad = 8 - (len % 8);
    pktoffs = 8 - pad;
    biglen = len + pad;		 

    for (i = pktoffs; i < 4+8; i++)
	pkt->data[i] = random_byte();
    crc = crc32_compute(pkt->data + pktoffs + 4, biglen - 4);  
    PUT_32BIT(pkt->data + pktoffs + 4 + biglen - 4, crc);
    PUT_32BIT(pkt->data + pktoffs, len);

    if (ssh->cipher)
	ssh->cipher->encrypt(ssh->v1_cipher_ctx,
			     pkt->data + pktoffs + 4, biglen);

    if (offset_p) *offset_p = pktoffs;
    return biglen + 4;		 
}
