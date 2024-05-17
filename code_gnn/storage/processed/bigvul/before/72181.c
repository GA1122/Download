mm_answer_sign(int sock, Buffer *m)
{
	struct ssh *ssh = active_state; 	 
	extern int auth_sock;			 
	struct sshkey *key;
	struct sshbuf *sigbuf = NULL;
	u_char *p = NULL, *signature = NULL;
	char *alg = NULL;
	size_t datlen, siglen, alglen;
	int r, is_proof = 0;
	u_int keyid;
	const char proof_req[] = "hostkeys-prove-00@openssh.com";

	debug3("%s", __func__);

	if ((r = sshbuf_get_u32(m, &keyid)) != 0 ||
	    (r = sshbuf_get_string(m, &p, &datlen)) != 0 ||
	    (r = sshbuf_get_cstring(m, &alg, &alglen)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));
	if (keyid > INT_MAX)
		fatal("%s: invalid key ID", __func__);

	 
	if (datlen != 20 && datlen != 32 && datlen != 48 && datlen != 64) {
		 
		if (session_id2_len == 0)  
			fatal("%s: bad data length: %zu", __func__, datlen);
		if ((key = get_hostkey_public_by_index(keyid, ssh)) == NULL)
			fatal("%s: no hostkey for index %d", __func__, keyid);
		if ((sigbuf = sshbuf_new()) == NULL)
			fatal("%s: sshbuf_new", __func__);
		if ((r = sshbuf_put_cstring(sigbuf, proof_req)) != 0 ||
		    (r = sshbuf_put_string(sigbuf, session_id2,
		    session_id2_len)) != 0 ||
		    (r = sshkey_puts(key, sigbuf)) != 0)
			fatal("%s: couldn't prepare private key "
			    "proof buffer: %s", __func__, ssh_err(r));
		if (datlen != sshbuf_len(sigbuf) ||
		    memcmp(p, sshbuf_ptr(sigbuf), sshbuf_len(sigbuf)) != 0)
			fatal("%s: bad data length: %zu, hostkey proof len %zu",
			    __func__, datlen, sshbuf_len(sigbuf));
		sshbuf_free(sigbuf);
		is_proof = 1;
	}

	 
	if (session_id2_len == 0) {
		session_id2_len = datlen;
		session_id2 = xmalloc(session_id2_len);
		memcpy(session_id2, p, session_id2_len);
	}

	if ((key = get_hostkey_by_index(keyid)) != NULL) {
		if ((r = sshkey_sign(key, &signature, &siglen, p, datlen, alg,
		    datafellows)) != 0)
			fatal("%s: sshkey_sign failed: %s",
			    __func__, ssh_err(r));
	} else if ((key = get_hostkey_public_by_index(keyid, ssh)) != NULL &&
	    auth_sock > 0) {
		if ((r = ssh_agent_sign(auth_sock, key, &signature, &siglen,
		    p, datlen, alg, datafellows)) != 0) {
			fatal("%s: ssh_agent_sign failed: %s",
			    __func__, ssh_err(r));
		}
	} else
		fatal("%s: no hostkey from index %d", __func__, keyid);

	debug3("%s: %s signature %p(%zu)", __func__,
	    is_proof ? "KEX" : "hostkey proof", signature, siglen);

	sshbuf_reset(m);
	if ((r = sshbuf_put_string(m, signature, siglen)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));

	free(alg);
	free(p);
	free(signature);

	mm_request_send(sock, MONITOR_ANS_SIGN, m);

	 
	monitor_permit(mon_dispatch, MONITOR_REQ_PWNAM, 1);

	return (0);
}