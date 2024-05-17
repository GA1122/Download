static int tls12_sigalg_allowed(SSL *s, int op, const unsigned char *ptmp)
	{
	 
	const tls12_hash_info *hinf = tls12_get_hash_info(ptmp[0]);
	if (!hinf || !hinf->mfunc)
		return 0;
	 
	if (tls12_get_pkey_idx(ptmp[1]) == -1)
		return 0;
	 
	return ssl_security(s, op, hinf->secbits, hinf->nid, (void *)ptmp);
	}
