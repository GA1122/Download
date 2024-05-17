sign_and_send_pubkey(Authctxt *authctxt, Identity *id)
{
	Buffer b;
	Identity *private_id;
	u_char *blob, *signature;
	size_t slen;
	u_int bloblen, skip = 0;
	int matched, ret = -1, have_sig = 1;
	char *fp;

	if ((fp = sshkey_fingerprint(id->key, options.fingerprint_hash,
	    SSH_FP_DEFAULT)) == NULL)
		return 0;
	debug3("%s: %s %s", __func__, key_type(id->key), fp);
	free(fp);

	if (key_to_blob(id->key, &blob, &bloblen) == 0) {
		 
		debug3("sign_and_send_pubkey: cannot handle key");
		return 0;
	}
	 
	buffer_init(&b);
	if (datafellows & SSH_OLD_SESSIONID) {
		buffer_append(&b, session_id2, session_id2_len);
		skip = session_id2_len;
	} else {
		buffer_put_string(&b, session_id2, session_id2_len);
		skip = buffer_len(&b);
	}
	buffer_put_char(&b, SSH2_MSG_USERAUTH_REQUEST);
	buffer_put_cstring(&b, authctxt->server_user);
	buffer_put_cstring(&b,
	    datafellows & SSH_BUG_PKSERVICE ?
	    "ssh-userauth" :
	    authctxt->service);
	if (datafellows & SSH_BUG_PKAUTH) {
		buffer_put_char(&b, have_sig);
	} else {
		buffer_put_cstring(&b, authctxt->method->name);
		buffer_put_char(&b, have_sig);
		buffer_put_cstring(&b, identity_sign_encode(id));
	}
	buffer_put_string(&b, blob, bloblen);

	 
	if (key_is_cert(id->key)) {
		matched = 0;
		TAILQ_FOREACH(private_id, &authctxt->keys, next) {
			if (sshkey_equal_public(id->key, private_id->key) &&
			    id->key->type != private_id->key->type) {
				id = private_id;
				matched = 1;
				break;
			}
		}
		if (matched) {
			debug2("%s: using private key \"%s\"%s for "
			    "certificate", __func__, id->filename,
			    id->agent_fd != -1 ? " from agent" : "");
		} else {
			debug("%s: no separate private key for certificate "
			    "\"%s\"", __func__, id->filename);
		}
	}

	 
	ret = identity_sign(id, &signature, &slen,
	    buffer_ptr(&b), buffer_len(&b), datafellows);
	if (ret != 0) {
		if (ret != SSH_ERR_KEY_NOT_FOUND)
			error("%s: signing failed: %s", __func__, ssh_err(ret));
		free(blob);
		buffer_free(&b);
		return 0;
	}
#ifdef DEBUG_PK
	buffer_dump(&b);
#endif
	if (datafellows & SSH_BUG_PKSERVICE) {
		buffer_clear(&b);
		buffer_append(&b, session_id2, session_id2_len);
		skip = session_id2_len;
		buffer_put_char(&b, SSH2_MSG_USERAUTH_REQUEST);
		buffer_put_cstring(&b, authctxt->server_user);
		buffer_put_cstring(&b, authctxt->service);
		buffer_put_cstring(&b, authctxt->method->name);
		buffer_put_char(&b, have_sig);
		if (!(datafellows & SSH_BUG_PKAUTH))
			buffer_put_cstring(&b, key_ssh_name(id->key));
		buffer_put_string(&b, blob, bloblen);
	}
	free(blob);

	 
	buffer_put_string(&b, signature, slen);
	free(signature);

	 
	if (buffer_len(&b) < skip + 1)
		fatal("userauth_pubkey: internal error");
	buffer_consume(&b, skip + 1);

	 
	packet_start(SSH2_MSG_USERAUTH_REQUEST);
	packet_put_raw(buffer_ptr(&b), buffer_len(&b));
	buffer_free(&b);
	packet_send();

	return 1;
}
