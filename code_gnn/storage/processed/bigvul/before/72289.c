notify_hostkeys(struct ssh *ssh)
{
	struct sshbuf *buf;
	struct sshkey *key;
	int i, nkeys, r;
	char *fp;

	 
	if (datafellows & SSH_BUG_HOSTKEYS)
		return;

	if ((buf = sshbuf_new()) == NULL)
		fatal("%s: sshbuf_new", __func__);
	for (i = nkeys = 0; i < options.num_host_key_files; i++) {
		key = get_hostkey_public_by_index(i, ssh);
		if (key == NULL || key->type == KEY_UNSPEC ||
		    sshkey_is_cert(key))
			continue;
		fp = sshkey_fingerprint(key, options.fingerprint_hash,
		    SSH_FP_DEFAULT);
		debug3("%s: key %d: %s %s", __func__, i,
		    sshkey_ssh_name(key), fp);
		free(fp);
		if (nkeys == 0) {
			packet_start(SSH2_MSG_GLOBAL_REQUEST);
			packet_put_cstring("hostkeys-00@openssh.com");
			packet_put_char(0);  
		}
		sshbuf_reset(buf);
		if ((r = sshkey_putb(key, buf)) != 0)
			fatal("%s: couldn't put hostkey %d: %s",
			    __func__, i, ssh_err(r));
		packet_put_string(sshbuf_ptr(buf), sshbuf_len(buf));
		nkeys++;
	}
	debug3("%s: sent %d hostkeys", __func__, nkeys);
	if (nkeys == 0)
		fatal("%s: no hostkeys", __func__);
	packet_send();
	sshbuf_free(buf);
}
