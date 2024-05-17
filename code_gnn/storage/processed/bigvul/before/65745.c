nfsd4_decode_exchange_id(struct nfsd4_compoundargs *argp,
			 struct nfsd4_exchange_id *exid)
{
	int dummy, tmp;
	DECODE_HEAD;

	READ_BUF(NFS4_VERIFIER_SIZE);
	COPYMEM(exid->verifier.data, NFS4_VERIFIER_SIZE);

	status = nfsd4_decode_opaque(argp, &exid->clname);
	if (status)
		return nfserr_bad_xdr;

	READ_BUF(4);
	exid->flags = be32_to_cpup(p++);

	 
	READ_BUF(4);
	exid->spa_how = be32_to_cpup(p++);
	switch (exid->spa_how) {
	case SP4_NONE:
		break;
	case SP4_MACH_CRED:
		 
		status = nfsd4_decode_bitmap(argp,
					exid->spo_must_enforce);
		if (status)
			goto out;
		 
		status = nfsd4_decode_bitmap(argp, exid->spo_must_allow);
		if (status)
			goto out;
		break;
	case SP4_SSV:
		 
		READ_BUF(4);
		dummy = be32_to_cpup(p++);
		READ_BUF(dummy * 4);
		p += dummy;

		READ_BUF(4);
		dummy = be32_to_cpup(p++);
		READ_BUF(dummy * 4);
		p += dummy;

		 
		READ_BUF(4);
		tmp = be32_to_cpup(p++);
		while (tmp--) {
			READ_BUF(4);
			dummy = be32_to_cpup(p++);
			READ_BUF(dummy);
			p += XDR_QUADLEN(dummy);
		}

		 
		READ_BUF(4);
		tmp = be32_to_cpup(p++);
		while (tmp--) {
			READ_BUF(4);
			dummy = be32_to_cpup(p++);
			READ_BUF(dummy);
			p += XDR_QUADLEN(dummy);
		}

		 
		READ_BUF(8);
		dummy = be32_to_cpup(p++);
		dummy = be32_to_cpup(p++);
		break;
	default:
		goto xdr_error;
	}

	 
	READ_BUF(4);     
	dummy = be32_to_cpup(p++);

	if (dummy > 1)
		goto xdr_error;

	if (dummy == 1) {
		 
		READ_BUF(4);
		dummy = be32_to_cpup(p++);
		READ_BUF(dummy);
		p += XDR_QUADLEN(dummy);

		 
		READ_BUF(4);
		dummy = be32_to_cpup(p++);
		READ_BUF(dummy);
		p += XDR_QUADLEN(dummy);

		 
		READ_BUF(12);
		p += 3;
	}
	DECODE_TAIL;
}
