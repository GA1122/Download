userauth_hostbased(Authctxt *authctxt)
{
	struct ssh *ssh = active_state;
	struct sshkey *private = NULL;
	struct sshbuf *b = NULL;
	const char *service;
	u_char *sig = NULL, *keyblob = NULL;
	char *fp = NULL, *chost = NULL, *lname = NULL;
	size_t siglen = 0, keylen = 0;
	int i, r, success = 0;

	if (authctxt->ktypes == NULL) {
		authctxt->oktypes = xstrdup(options.hostbased_key_types);
		authctxt->ktypes = authctxt->oktypes;
	}

	 
	for (;;) {
		if (authctxt->active_ktype == NULL)
			authctxt->active_ktype = strsep(&authctxt->ktypes, ",");
		if (authctxt->active_ktype == NULL ||
		    *authctxt->active_ktype == '\0')
			break;
		debug3("%s: trying key type %s", __func__,
		    authctxt->active_ktype);

		 
		private = NULL;
		for (i = 0; i < authctxt->sensitive->nkeys; i++) {
			if (authctxt->sensitive->keys[i] == NULL ||
			    authctxt->sensitive->keys[i]->type == KEY_RSA1 ||
			    authctxt->sensitive->keys[i]->type == KEY_UNSPEC)
				continue;
			if (match_pattern_list(
			    sshkey_ssh_name(authctxt->sensitive->keys[i]),
			    authctxt->active_ktype, 0) != 1)
				continue;
			 
			private = authctxt->sensitive->keys[i];
			authctxt->sensitive->keys[i] = NULL;
			break;
		}
		 
		if (private != NULL)
			break;
		 
		authctxt->active_ktype = NULL;
	}
	if (private == NULL) {
		free(authctxt->oktypes);
		authctxt->oktypes = authctxt->ktypes = NULL;
		authctxt->active_ktype = NULL;
		debug("No more client hostkeys for hostbased authentication.");
		goto out;
	}

	if ((fp = sshkey_fingerprint(private, options.fingerprint_hash,
	    SSH_FP_DEFAULT)) == NULL) {
		error("%s: sshkey_fingerprint failed", __func__);
		goto out;
	}
	debug("%s: trying hostkey %s %s",
	    __func__, sshkey_ssh_name(private), fp);

	 
	if ((lname = get_local_name(packet_get_connection_in())) == NULL) {
		error("%s: cannot get local ipaddr/name", __func__);
		goto out;
	}

	 
	xasprintf(&chost, "%s.", lname);
	debug2("%s: chost %s", __func__, chost);

	service = datafellows & SSH_BUG_HBSERVICE ? "ssh-userauth" :
	    authctxt->service;

	 
	if ((b = sshbuf_new()) == NULL) {
		error("%s: sshbuf_new failed", __func__);
		goto out;
	}
	if ((r = sshkey_to_blob(private, &keyblob, &keylen)) != 0) {
		error("%s: sshkey_to_blob: %s", __func__, ssh_err(r));
		goto out;
	}
	if ((r = sshbuf_put_string(b, session_id2, session_id2_len)) != 0 ||
	    (r = sshbuf_put_u8(b, SSH2_MSG_USERAUTH_REQUEST)) != 0 ||
	    (r = sshbuf_put_cstring(b, authctxt->server_user)) != 0 ||
	    (r = sshbuf_put_cstring(b, service)) != 0 ||
	    (r = sshbuf_put_cstring(b, authctxt->method->name)) != 0 ||
	    (r = sshbuf_put_cstring(b, key_ssh_name(private))) != 0 ||
	    (r = sshbuf_put_string(b, keyblob, keylen)) != 0 ||
	    (r = sshbuf_put_cstring(b, chost)) != 0 ||
	    (r = sshbuf_put_cstring(b, authctxt->local_user)) != 0) {
		error("%s: buffer error: %s", __func__, ssh_err(r));
		goto out;
	}

#ifdef DEBUG_PK
	sshbuf_dump(b, stderr);
#endif
	if (authctxt->sensitive->external_keysign)
		r = ssh_keysign(private, &sig, &siglen,
		    sshbuf_ptr(b), sshbuf_len(b));
	else if ((r = sshkey_sign(private, &sig, &siglen,
	    sshbuf_ptr(b), sshbuf_len(b), NULL, datafellows)) != 0)
		debug("%s: sshkey_sign: %s", __func__, ssh_err(r));
	if (r != 0) {
		error("sign using hostkey %s %s failed",
		    sshkey_ssh_name(private), fp);
		goto out;
	}
	if ((r = sshpkt_start(ssh, SSH2_MSG_USERAUTH_REQUEST)) != 0 ||
	    (r = sshpkt_put_cstring(ssh, authctxt->server_user)) != 0 ||
	    (r = sshpkt_put_cstring(ssh, authctxt->service)) != 0 ||
	    (r = sshpkt_put_cstring(ssh, authctxt->method->name)) != 0 ||
	    (r = sshpkt_put_cstring(ssh, key_ssh_name(private))) != 0 ||
	    (r = sshpkt_put_string(ssh, keyblob, keylen)) != 0 ||
	    (r = sshpkt_put_cstring(ssh, chost)) != 0 ||
	    (r = sshpkt_put_cstring(ssh, authctxt->local_user)) != 0 ||
	    (r = sshpkt_put_string(ssh, sig, siglen)) != 0 ||
	    (r = sshpkt_send(ssh)) != 0) {
		error("%s: packet error: %s", __func__, ssh_err(r));
		goto out;
	}
	success = 1;

 out:
	if (sig != NULL) {
		explicit_bzero(sig, siglen);
		free(sig);
	}
	free(keyblob);
	free(lname);
	free(fp);
	free(chost);
	sshkey_free(private);
	sshbuf_free(b);

	return success;
}
