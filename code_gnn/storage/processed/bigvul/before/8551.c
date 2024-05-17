static void ssh2_pkt_send_with_padding(Ssh ssh, struct Packet *pkt,
				       int padsize)
{
#if 0
    if (0) {
	 
	pkt->forcepad = padsize;
	ssh2_pkt_send(ssh, pkt);
    } else
#endif
    {
	 
	ssh2_pkt_defer(ssh, pkt);

	 
	if (ssh->cscipher &&
	    !(ssh->remote_bugs & BUG_CHOKES_ON_SSH2_IGNORE)) {
	    int stringlen, i;

	    stringlen = (256 - ssh->deferred_len);
	    stringlen += ssh->cscipher->blksize - 1;
	    stringlen -= (stringlen % ssh->cscipher->blksize);
	    if (ssh->cscomp) {
		 
		stringlen -=
		    ssh->cscomp->disable_compression(ssh->cs_comp_ctx);
	    }
	    pkt = ssh2_pkt_init(SSH2_MSG_IGNORE);
	    ssh2_pkt_addstring_start(pkt);
	    for (i = 0; i < stringlen; i++) {
		char c = (char) random_byte();
		ssh2_pkt_addstring_data(pkt, &c, 1);
	    }
	    ssh2_pkt_defer(ssh, pkt);
	}
	ssh_pkt_defersend(ssh);
    }
}
