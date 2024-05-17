nfsd4_encode_exchange_id(struct nfsd4_compoundres *resp, __be32 nfserr,
			 struct nfsd4_exchange_id *exid)
{
	struct xdr_stream *xdr = &resp->xdr;
	__be32 *p;
	char *major_id;
	char *server_scope;
	int major_id_sz;
	int server_scope_sz;
	int status = 0;
	uint64_t minor_id = 0;

	if (nfserr)
		return nfserr;

	major_id = utsname()->nodename;
	major_id_sz = strlen(major_id);
	server_scope = utsname()->nodename;
	server_scope_sz = strlen(server_scope);

	p = xdr_reserve_space(xdr,
		8   +
		4   +
		4   +
		4  );
	if (!p)
		return nfserr_resource;

	p = xdr_encode_opaque_fixed(p, &exid->clientid, 8);
	*p++ = cpu_to_be32(exid->seqid);
	*p++ = cpu_to_be32(exid->flags);

	*p++ = cpu_to_be32(exid->spa_how);

	switch (exid->spa_how) {
	case SP4_NONE:
		break;
	case SP4_MACH_CRED:
		 
		status = nfsd4_encode_bitmap(xdr,
					exid->spo_must_enforce[0],
					exid->spo_must_enforce[1],
					exid->spo_must_enforce[2]);
		if (status)
			goto out;
		 
		status = nfsd4_encode_bitmap(xdr,
					exid->spo_must_allow[0],
					exid->spo_must_allow[1],
					exid->spo_must_allow[2]);
		if (status)
			goto out;
		break;
	default:
		WARN_ON_ONCE(1);
	}

	p = xdr_reserve_space(xdr,
		8   +
		4   +
		(XDR_QUADLEN(major_id_sz) * 4) +
		4   +
		(XDR_QUADLEN(server_scope_sz) * 4) +
		4  );
	if (!p)
		return nfserr_resource;

	 
	p = xdr_encode_hyper(p, minor_id);       
	 
	p = xdr_encode_opaque(p, major_id, major_id_sz);

	 
	p = xdr_encode_opaque(p, server_scope, server_scope_sz);

	 
	*p++ = cpu_to_be32(0);	 
	return 0;
out:
	return status;
}
