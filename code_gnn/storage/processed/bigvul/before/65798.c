nfsd4_encode_create_session(struct nfsd4_compoundres *resp, __be32 nfserr,
			    struct nfsd4_create_session *sess)
{
	struct xdr_stream *xdr = &resp->xdr;
	__be32 *p;

	if (nfserr)
		return nfserr;

	p = xdr_reserve_space(xdr, 24);
	if (!p)
		return nfserr_resource;
	p = xdr_encode_opaque_fixed(p, sess->sessionid.data,
					NFS4_MAX_SESSIONID_LEN);
	*p++ = cpu_to_be32(sess->seqid);
	*p++ = cpu_to_be32(sess->flags);

	p = xdr_reserve_space(xdr, 28);
	if (!p)
		return nfserr_resource;
	*p++ = cpu_to_be32(0);  
	*p++ = cpu_to_be32(sess->fore_channel.maxreq_sz);
	*p++ = cpu_to_be32(sess->fore_channel.maxresp_sz);
	*p++ = cpu_to_be32(sess->fore_channel.maxresp_cached);
	*p++ = cpu_to_be32(sess->fore_channel.maxops);
	*p++ = cpu_to_be32(sess->fore_channel.maxreqs);
	*p++ = cpu_to_be32(sess->fore_channel.nr_rdma_attrs);

	if (sess->fore_channel.nr_rdma_attrs) {
		p = xdr_reserve_space(xdr, 4);
		if (!p)
			return nfserr_resource;
		*p++ = cpu_to_be32(sess->fore_channel.rdma_attrs);
	}

	p = xdr_reserve_space(xdr, 28);
	if (!p)
		return nfserr_resource;
	*p++ = cpu_to_be32(0);  
	*p++ = cpu_to_be32(sess->back_channel.maxreq_sz);
	*p++ = cpu_to_be32(sess->back_channel.maxresp_sz);
	*p++ = cpu_to_be32(sess->back_channel.maxresp_cached);
	*p++ = cpu_to_be32(sess->back_channel.maxops);
	*p++ = cpu_to_be32(sess->back_channel.maxreqs);
	*p++ = cpu_to_be32(sess->back_channel.nr_rdma_attrs);

	if (sess->back_channel.nr_rdma_attrs) {
		p = xdr_reserve_space(xdr, 4);
		if (!p)
			return nfserr_resource;
		*p++ = cpu_to_be32(sess->back_channel.rdma_attrs);
	}
	return 0;
}