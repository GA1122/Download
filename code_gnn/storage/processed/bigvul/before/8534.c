static void ssh2_add_sigblob(Ssh ssh, struct Packet *pkt,
			     void *pkblob_v, int pkblob_len,
			     void *sigblob_v, int sigblob_len)
{
    unsigned char *pkblob = (unsigned char *)pkblob_v;
    unsigned char *sigblob = (unsigned char *)sigblob_v;

     
     

     
    if ((ssh->remote_bugs & BUG_SSH2_RSA_PADDING) && pkblob_len > 4+7+4 &&
	(GET_32BIT(pkblob) == 7 && !memcmp(pkblob+4, "ssh-rsa", 7))) {
	int pos, len, siglen;

	 

	pos = 4+7;		        
        len = toint(GET_32BIT(pkblob+pos));  
        if (len < 0 || len > pkblob_len - pos - 4)
            goto give_up;
	pos += 4 + len;                 
        if (pkblob_len - pos < 4)
            goto give_up;
	len = toint(GET_32BIT(pkblob+pos));  
        if (len < 0 || len > pkblob_len - pos - 4)
            goto give_up;
	pos += 4;		        
	while (len > 0 && pkblob[pos] == 0)
	    len--, pos++;
	 

	 
	pos = 4+7;		        
        if (sigblob_len < pos+4)
            goto give_up;
	siglen = toint(GET_32BIT(sigblob+pos));
        if (siglen != sigblob_len - pos - 4)
            goto give_up;
	 

	if (len != siglen) {
	    unsigned char newlen[4];
	    ssh2_pkt_addstring_start(pkt);
	    ssh2_pkt_addstring_data(pkt, (char *)sigblob, pos);
	     
	    pos += 4;		        
	    PUT_32BIT(newlen, len);
	    ssh2_pkt_addstring_data(pkt, (char *)newlen, 4);
	     
	    newlen[0] = 0;
	    while (len-- > siglen) {
		ssh2_pkt_addstring_data(pkt, (char *)newlen, 1);
		 
	    }
	    ssh2_pkt_addstring_data(pkt, (char *)(sigblob+pos), siglen);
	     
	    return;
	}

	 
      give_up:;
    }

    ssh2_pkt_addstring_start(pkt);
    ssh2_pkt_addstring_data(pkt, (char *)sigblob, sigblob_len);
}
