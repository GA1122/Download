static __be32 nfsd4_encode_bind_conn_to_session(struct nfsd4_compoundres *resp, __be32 nfserr, struct nfsd4_bind_conn_to_session *bcts)
{
	struct xdr_stream *xdr = &resp->xdr;
	__be32 *p;

	if (!nfserr) {
		p = xdr_reserve_space(xdr, NFS4_MAX_SESSIONID_LEN + 8);
		if (!p)
			return nfserr_resource;
		p = xdr_encode_opaque_fixed(p, bcts->sessionid.data,
						NFS4_MAX_SESSIONID_LEN);
		*p++ = cpu_to_be32(bcts->dir);
		 
		*p++ = cpu_to_be32(0);
	}
	return nfserr;
}