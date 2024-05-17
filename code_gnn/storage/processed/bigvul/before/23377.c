static inline void encode_openhdr(struct xdr_stream *xdr, const struct nfs_openargs *arg)
{
	__be32 *p;
  
	p = reserve_space(xdr, 8);
	*p++ = cpu_to_be32(OP_OPEN);
	*p = cpu_to_be32(arg->seqid->sequence->counter);
	encode_share_access(xdr, arg->fmode);
	p = reserve_space(xdr, 32);
	p = xdr_encode_hyper(p, arg->clientid);
	*p++ = cpu_to_be32(20);
	p = xdr_encode_opaque_fixed(p, "open id:", 8);
	*p++ = cpu_to_be32(arg->server->s_dev);
	xdr_encode_hyper(p, arg->id);
}