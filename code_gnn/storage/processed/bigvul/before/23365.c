encode_layoutget(struct xdr_stream *xdr,
		      const struct nfs4_layoutget_args *args,
		      struct compound_hdr *hdr)
{
	__be32 *p;

	p = reserve_space(xdr, 44 + NFS4_STATEID_SIZE);
	*p++ = cpu_to_be32(OP_LAYOUTGET);
	*p++ = cpu_to_be32(0);      
	*p++ = cpu_to_be32(args->type);
	*p++ = cpu_to_be32(args->range.iomode);
	p = xdr_encode_hyper(p, args->range.offset);
	p = xdr_encode_hyper(p, args->range.length);
	p = xdr_encode_hyper(p, args->minlength);
	p = xdr_encode_opaque_fixed(p, &args->stateid.data, NFS4_STATEID_SIZE);
	*p = cpu_to_be32(args->maxcount);

	dprintk("%s: 1st type:0x%x iomode:%d off:%lu len:%lu mc:%d\n",
		__func__,
		args->type,
		args->range.iomode,
		(unsigned long)args->range.offset,
		(unsigned long)args->range.length,
		args->maxcount);
	hdr->nops++;
	hdr->replen += decode_layoutget_maxsz;
}