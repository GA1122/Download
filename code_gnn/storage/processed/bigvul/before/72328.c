server_input_hostkeys_prove(struct sshbuf **respp)
{
	struct ssh *ssh = active_state;  
	struct sshbuf *resp = NULL;
	struct sshbuf *sigbuf = NULL;
	struct sshkey *key = NULL, *key_pub = NULL, *key_prv = NULL;
	int r, ndx, success = 0;
	const u_char *blob;
	u_char *sig = 0;
	size_t blen, slen;

	if ((resp = sshbuf_new()) == NULL || (sigbuf = sshbuf_new()) == NULL)
		fatal("%s: sshbuf_new", __func__);

	while (ssh_packet_remaining(ssh) > 0) {
		sshkey_free(key);
		key = NULL;
		if ((r = sshpkt_get_string_direct(ssh, &blob, &blen)) != 0 ||
		    (r = sshkey_from_blob(blob, blen, &key)) != 0) {
			error("%s: couldn't parse key: %s",
			    __func__, ssh_err(r));
			goto out;
		}
		 
		if ((ndx = ssh->kex->host_key_index(key, 1, ssh)) == -1) {
			error("%s: unknown host %s key",
			    __func__, sshkey_type(key));
			goto out;
		}
		 
		if ((key_prv = get_hostkey_by_index(ndx)) == NULL &&
		    (key_pub = get_hostkey_public_by_index(ndx, ssh)) == NULL) {
			error("%s: can't retrieve hostkey %d", __func__, ndx);
			goto out;
		}
		sshbuf_reset(sigbuf);
		free(sig);
		sig = NULL;
		if ((r = sshbuf_put_cstring(sigbuf,
		    "hostkeys-prove-00@openssh.com")) != 0 ||
		    (r = sshbuf_put_string(sigbuf,
		    ssh->kex->session_id, ssh->kex->session_id_len)) != 0 ||
		    (r = sshkey_puts(key, sigbuf)) != 0 ||
		    (r = ssh->kex->sign(key_prv, key_pub, &sig, &slen,
		    sshbuf_ptr(sigbuf), sshbuf_len(sigbuf), NULL, 0)) != 0 ||
		    (r = sshbuf_put_string(resp, sig, slen)) != 0) {
			error("%s: couldn't prepare signature: %s",
			    __func__, ssh_err(r));
			goto out;
		}
	}
	 
	*respp = resp;
	resp = NULL;  
	success = 1;
 out:
	free(sig);
	sshbuf_free(resp);
	sshbuf_free(sigbuf);
	sshkey_free(key);
	return success;
}