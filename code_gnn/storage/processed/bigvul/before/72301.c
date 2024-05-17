sshd_hostkey_sign(Key *privkey, Key *pubkey, u_char **signature, size_t *slen,
    const u_char *data, size_t dlen, const char *alg, u_int flag)
{
	int r;
	u_int xxx_slen, xxx_dlen = dlen;

	if (privkey) {
		if (PRIVSEP(key_sign(privkey, signature, &xxx_slen, data, xxx_dlen,
		    alg) < 0))
			fatal("%s: key_sign failed", __func__);
		if (slen)
			*slen = xxx_slen;
	} else if (use_privsep) {
		if (mm_key_sign(pubkey, signature, &xxx_slen, data, xxx_dlen,
		    alg) < 0)
			fatal("%s: pubkey_sign failed", __func__);
		if (slen)
			*slen = xxx_slen;
	} else {
		if ((r = ssh_agent_sign(auth_sock, pubkey, signature, slen,
		    data, dlen, alg, datafellows)) != 0)
			fatal("%s: ssh_agent_sign failed: %s",
			    __func__, ssh_err(r));
	}
	return 0;
}
