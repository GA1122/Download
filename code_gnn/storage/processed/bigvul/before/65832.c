nfsd4_encode_security_label(struct xdr_stream *xdr, struct svc_rqst *rqstp,
			    void *context, int len)
{
	__be32 *p;

	p = xdr_reserve_space(xdr, len + 4 + 4 + 4);
	if (!p)
		return nfserr_resource;

	 
	*p++ = cpu_to_be32(0);  
	*p++ = cpu_to_be32(0);  
	p = xdr_encode_opaque(p, context, len);
	return 0;
}
