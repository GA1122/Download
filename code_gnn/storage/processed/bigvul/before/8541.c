static int ssh2_pkt_construct(Ssh ssh, struct Packet *pkt)
{
    int cipherblk, maclen, padding, unencrypted_prefix, i;

    if (ssh->logctx)
        ssh2_log_outgoing_packet(ssh, pkt);

    if (ssh->bare_connection) {
         
        PUT_32BIT(pkt->data + 1, pkt->length - 5);
        pkt->body = pkt->data + 1;
        ssh->v2_outgoing_sequence++;    
        return pkt->length - 1;
    }

     
    {
	unsigned char *newpayload;
	int newlen;
	if (ssh->cscomp &&
	    ssh->cscomp->compress(ssh->cs_comp_ctx, pkt->data + 5,
				  pkt->length - 5,
				  &newpayload, &newlen)) {
	    pkt->length = 5;
	    ssh2_pkt_adddata(pkt, newpayload, newlen);
	    sfree(newpayload);
	}
    }

     
    cipherblk = ssh->cscipher ? ssh->cscipher->blksize : 8;   
    cipherblk = cipherblk < 8 ? 8 : cipherblk;	 
    padding = 4;
    unencrypted_prefix = (ssh->csmac && ssh->csmac_etm) ? 4 : 0;
    if (pkt->length + padding < pkt->forcepad)
	padding = pkt->forcepad - pkt->length;
    padding +=
	(cipherblk - (pkt->length - unencrypted_prefix + padding) % cipherblk)
        % cipherblk;
    assert(padding <= 255);
    maclen = ssh->csmac ? ssh->csmac->len : 0;
    ssh2_pkt_ensure(pkt, pkt->length + padding + maclen);
    pkt->data[4] = padding;
    for (i = 0; i < padding; i++)
	pkt->data[pkt->length + i] = random_byte();
    PUT_32BIT(pkt->data, pkt->length + padding - 4);

     
    if (ssh->cscipher && (ssh->cscipher->flags & SSH_CIPHER_SEPARATE_LENGTH)) {
        ssh->cscipher->encrypt_length(ssh->cs_cipher_ctx, pkt->data, 4,
                                      ssh->v2_outgoing_sequence);
    }

    if (ssh->csmac && ssh->csmac_etm) {
         
        if (ssh->cscipher)
            ssh->cscipher->encrypt(ssh->cs_cipher_ctx,
                                   pkt->data + 4, pkt->length + padding - 4);
        ssh->csmac->generate(ssh->cs_mac_ctx, pkt->data,
                             pkt->length + padding,
                             ssh->v2_outgoing_sequence);
    } else {
         
        if (ssh->csmac)
            ssh->csmac->generate(ssh->cs_mac_ctx, pkt->data,
                                 pkt->length + padding,
                                 ssh->v2_outgoing_sequence);
        if (ssh->cscipher)
            ssh->cscipher->encrypt(ssh->cs_cipher_ctx,
                                   pkt->data, pkt->length + padding);
    }

    ssh->v2_outgoing_sequence++;        
    pkt->encrypted_len = pkt->length + padding;

     
    pkt->body = pkt->data;
    return pkt->length + padding + maclen;
}
