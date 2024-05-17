identity_sign(struct identity *id, u_char **sigp, size_t *lenp,
    const u_char *data, size_t datalen, u_int compat)
{
	Key *prv;
	int ret;
	const char *alg;

	alg = identity_sign_encode(id);

	 
	if (id->agent_fd != -1)
		return ssh_agent_sign(id->agent_fd, id->key, sigp, lenp,
		    data, datalen, alg, compat);

	 
	if (id->isprivate || (id->key->flags & SSHKEY_FLAG_EXT))
		return (sshkey_sign(id->key, sigp, lenp, data, datalen, alg,
		    compat));
	 
	if ((prv = load_identity_file(id)) == NULL)
		return SSH_ERR_KEY_NOT_FOUND;
	ret = sshkey_sign(prv, sigp, lenp, data, datalen, alg, compat);
	sshkey_free(prv);
	return (ret);
}
