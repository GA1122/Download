process_authentication_challenge1(SocketEntry *e)
{
	u_char buf[32], mdbuf[16], session_id[16];
	u_int response_type;
	BIGNUM *challenge;
	Identity *id;
	int r, len;
	struct sshbuf *msg;
	struct ssh_digest_ctx *md;
	struct sshkey *key;

	if ((msg = sshbuf_new()) == NULL)
		fatal("%s: sshbuf_new failed", __func__);
	if ((key = sshkey_new(KEY_RSA1)) == NULL)
		fatal("%s: sshkey_new failed", __func__);
	if ((challenge = BN_new()) == NULL)
		fatal("%s: BN_new failed", __func__);

	if ((r = sshbuf_get_u32(e->request, NULL)) != 0 ||  
	    (r = sshbuf_get_bignum1(e->request, key->rsa->e)) != 0 ||
	    (r = sshbuf_get_bignum1(e->request, key->rsa->n)) != 0 ||
	    (r = sshbuf_get_bignum1(e->request, challenge)))
		fatal("%s: buffer error: %s", __func__, ssh_err(r));

	 
	if (sshbuf_len(e->request) == 0)
		goto failure;
	if ((r = sshbuf_get(e->request, session_id, sizeof(session_id))) != 0 ||
	    (r = sshbuf_get_u32(e->request, &response_type)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));
	if (response_type != 1)
		goto failure;

	id = lookup_identity(key, 1);
	if (id != NULL && (!id->confirm || confirm_key(id) == 0)) {
		struct sshkey *private = id->key;
		 
		if ((r = rsa_private_decrypt(challenge, challenge,
		    private->rsa) != 0)) {
			fatal("%s: rsa_public_encrypt: %s", __func__,
			    ssh_err(r));
			goto failure;	 
		}

		 
		len = BN_num_bytes(challenge);
		if (len <= 0 || len > 32) {
			logit("%s: bad challenge length %d", __func__, len);
			goto failure;
		}
		memset(buf, 0, 32);
		BN_bn2bin(challenge, buf + 32 - len);
		if ((md = ssh_digest_start(SSH_DIGEST_MD5)) == NULL ||
		    ssh_digest_update(md, buf, 32) < 0 ||
		    ssh_digest_update(md, session_id, 16) < 0 ||
		    ssh_digest_final(md, mdbuf, sizeof(mdbuf)) < 0)
			fatal("%s: md5 failed", __func__);
		ssh_digest_free(md);

		 
		if ((r = sshbuf_put_u8(msg, SSH_AGENT_RSA_RESPONSE)) != 0 ||
		    (r = sshbuf_put(msg, mdbuf, sizeof(mdbuf))) != 0)
			fatal("%s: buffer error: %s", __func__, ssh_err(r));
		goto send;
	}

 failure:
	 
	if ((r = sshbuf_put_u8(msg, SSH_AGENT_FAILURE)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));
 send:
	if ((r = sshbuf_put_stringb(e->output, msg)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));
	sshkey_free(key);
	BN_clear_free(challenge);
	sshbuf_free(msg);
}