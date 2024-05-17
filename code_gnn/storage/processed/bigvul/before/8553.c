static struct Packet *ssh2_rdpkt(Ssh ssh, const unsigned char **data,
                                 int *datalen)
{
    struct rdpkt2_state_tag *st = &ssh->rdpkt2_state;

    crBegin(ssh->ssh2_rdpkt_crstate);

    st->pktin = ssh_new_packet();

    st->pktin->type = 0;
    st->pktin->length = 0;
    if (ssh->sccipher)
	st->cipherblk = ssh->sccipher->blksize;
    else
	st->cipherblk = 8;
    if (st->cipherblk < 8)
	st->cipherblk = 8;
    st->maclen = ssh->scmac ? ssh->scmac->len : 0;

    if (ssh->sccipher && (ssh->sccipher->flags & SSH_CIPHER_IS_CBC) &&
	ssh->scmac && !ssh->scmac_etm) {
	 

	 
	st->pktin->data = snewn(OUR_V2_PACKETLIMIT + st->maclen + APIEXTRA,
				unsigned char);

	 
	for (st->i = 0; st->i < st->maclen; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}

	st->packetlen = 0;
	{
	    unsigned char seq[4];
	    ssh->scmac->start(ssh->sc_mac_ctx);
	    PUT_32BIT(seq, st->incoming_sequence);
	    ssh->scmac->bytes(ssh->sc_mac_ctx, seq, 4);
	}

	for (;;) {  
	     
	    for (st->i = 0; st->i < st->cipherblk; st->i++) {
		while ((*datalen) == 0)
		    crReturn(NULL);
		st->pktin->data[st->packetlen+st->maclen+st->i] = *(*data)++;
		(*datalen)--;
	    }
	     
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data + st->packetlen,
				   st->cipherblk);
	     
	    ssh->scmac->bytes(ssh->sc_mac_ctx,
			      st->pktin->data + st->packetlen, st->cipherblk);
	    st->packetlen += st->cipherblk;
	     
	    if (ssh->scmac->verresult(ssh->sc_mac_ctx,
				      st->pktin->data + st->packetlen) &&
		((st->len = toint(GET_32BIT(st->pktin->data))) ==
                 st->packetlen-4))
		    break;
	    if (st->packetlen >= OUR_V2_PACKETLIMIT) {
		bombout(("No valid incoming packet found"));
		ssh_free_packet(st->pktin);
		crStop(NULL);
	    }	    
	}
	st->pktin->maxlen = st->packetlen + st->maclen;
	st->pktin->data = sresize(st->pktin->data,
				  st->pktin->maxlen + APIEXTRA,
				  unsigned char);
    } else if (ssh->scmac && ssh->scmac_etm) {
	st->pktin->data = snewn(4 + APIEXTRA, unsigned char);

         
	for (st->i = st->len = 0; st->i < 4; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}
         
        if (ssh->sccipher && (ssh->sccipher->flags & SSH_CIPHER_SEPARATE_LENGTH)) {
             
            unsigned char len[4];
            memcpy(len, st->pktin->data, 4);
            ssh->sccipher->decrypt_length(ssh->sc_cipher_ctx, len, 4, st->incoming_sequence);
            st->len = toint(GET_32BIT(len));
        } else {
            st->len = toint(GET_32BIT(st->pktin->data));
        }

	 
	if (st->len < 0 || st->len > OUR_V2_PACKETLIMIT ||
	    st->len % st->cipherblk != 0) {
	    bombout(("Incoming packet length field was garbled"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}

	 
	st->packetlen = st->len + 4;

	 
	st->pktin->maxlen = st->packetlen + st->maclen;
	st->pktin->data = sresize(st->pktin->data,
				  st->pktin->maxlen + APIEXTRA,
				  unsigned char);

	 
	for (st->i = 4; st->i < st->packetlen + st->maclen; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}

	 
	if (ssh->scmac
	    && !ssh->scmac->verify(ssh->sc_mac_ctx, st->pktin->data,
				   st->len + 4, st->incoming_sequence)) {
	    bombout(("Incorrect MAC received on packet"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}

	 
	if (ssh->sccipher)
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data + 4,
				   st->packetlen - 4);
    } else {
	st->pktin->data = snewn(st->cipherblk + APIEXTRA, unsigned char);

	 
	for (st->i = st->len = 0; st->i < st->cipherblk; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}

	if (ssh->sccipher)
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data, st->cipherblk);

	 
	st->len = toint(GET_32BIT(st->pktin->data));

	 
	if (st->len < 0 || st->len > OUR_V2_PACKETLIMIT ||
	    (st->len + 4) % st->cipherblk != 0) {
	    bombout(("Incoming packet was garbled on decryption"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}

	 
	st->packetlen = st->len + 4;

	 
	st->pktin->maxlen = st->packetlen + st->maclen;
	st->pktin->data = sresize(st->pktin->data,
				  st->pktin->maxlen + APIEXTRA,
				  unsigned char);

	 
	for (st->i = st->cipherblk; st->i < st->packetlen + st->maclen;
	     st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}
	 
	if (ssh->sccipher)
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data + st->cipherblk,
				   st->packetlen - st->cipherblk);

	 
	if (ssh->scmac
	    && !ssh->scmac->verify(ssh->sc_mac_ctx, st->pktin->data,
				   st->len + 4, st->incoming_sequence)) {
	    bombout(("Incorrect MAC received on packet"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}
    }
     
    st->pad = st->pktin->data[4];
    if (st->pad < 4 || st->len - st->pad < 1) {
	bombout(("Invalid padding length on received packet"));
	ssh_free_packet(st->pktin);
	crStop(NULL);
    }
     
    st->payload = st->len - st->pad - 1;

    st->pktin->length = st->payload + 5;
    st->pktin->encrypted_len = st->packetlen;

    st->pktin->sequence = st->incoming_sequence++;

    st->pktin->length = st->packetlen - st->pad;
    assert(st->pktin->length >= 0);

     
    {
	unsigned char *newpayload;
	int newlen;
	if (ssh->sccomp &&
	    ssh->sccomp->decompress(ssh->sc_comp_ctx,
				    st->pktin->data + 5, st->pktin->length - 5,
				    &newpayload, &newlen)) {
	    if (st->pktin->maxlen < newlen + 5) {
		st->pktin->maxlen = newlen + 5;
		st->pktin->data = sresize(st->pktin->data,
					  st->pktin->maxlen + APIEXTRA,
					  unsigned char);
	    }
	    st->pktin->length = 5 + newlen;
	    memcpy(st->pktin->data + 5, newpayload, newlen);
	    sfree(newpayload);
	}
    }

     
    if (st->pktin->length <= 5) {  
        ssh2_msg_something_unimplemented(ssh, st->pktin);
        crStop(NULL);
    }
     
    st->pktin->type = st->pktin->data[5];
    st->pktin->body = st->pktin->data + 6;
    st->pktin->length -= 6;
    assert(st->pktin->length >= 0);     

    if (ssh->logctx)
        ssh2_log_incoming_packet(ssh, st->pktin);

    st->pktin->savedpos = 0;

    crFinish(st->pktin);
}
