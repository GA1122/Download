 kex_input_kexinit(int type, u_int32_t seq, void *ctxt)
 {
 	struct ssh *ssh = ctxt;
 	struct kex *kex = ssh->kex;
 	const u_char *ptr;
 	u_int i;
 	size_t dlen;
 	int r;
 
 	debug("SSH2_MSG_KEXINIT received");
  	if (kex == NULL)
  		return SSH_ERR_INVALID_ARGUMENT;
  
// 	ssh_dispatch_set(ssh, SSH2_MSG_KEXINIT, NULL);
  	ptr = sshpkt_ptr(ssh, &dlen);
  	if ((r = sshbuf_put(kex->peer, ptr, dlen)) != 0)
  		return r;
 
 	 
 	for (i = 0; i < KEX_COOKIE_LEN; i++)
 		if ((r = sshpkt_get_u8(ssh, NULL)) != 0)
 			return r;
 	for (i = 0; i < PROPOSAL_MAX; i++)
 		if ((r = sshpkt_get_string(ssh, NULL, NULL)) != 0)
 			return r;
 	 
 	if ((r = sshpkt_get_u8(ssh, NULL)) != 0 ||	 
 	    (r = sshpkt_get_u32(ssh, NULL)) != 0 ||	 
 	    (r = sshpkt_get_end(ssh)) != 0)
 			return r;
 
 	if (!(kex->flags & KEX_INIT_SENT))
 		if ((r = kex_send_kexinit(ssh)) != 0)
 			return r;
 	if ((r = kex_choose_conf(ssh)) != 0)
 		return r;
 
 	if (kex->kex_type < KEX_MAX && kex->kex[kex->kex_type] != NULL)
 		return (kex->kex[kex->kex_type])(ssh);
 
 	return SSH_ERR_INTERNAL_ERROR;
 }