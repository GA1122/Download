encode_getdevicelist(struct xdr_stream *xdr,
		     const struct nfs4_getdevicelist_args *args,
		     struct compound_hdr *hdr)
{
	__be32 *p;
	nfs4_verifier dummy = {
		.data = "dummmmmy",
	};

	p = reserve_space(xdr, 20);
	*p++ = cpu_to_be32(OP_GETDEVICELIST);
	*p++ = cpu_to_be32(args->layoutclass);
	*p++ = cpu_to_be32(NFS4_PNFS_GETDEVLIST_MAXNUM);
	xdr_encode_hyper(p, 0ULL);                           
	encode_nfs4_verifier(xdr, &dummy);
	hdr->nops++;
	hdr->replen += decode_getdevicelist_maxsz;
}