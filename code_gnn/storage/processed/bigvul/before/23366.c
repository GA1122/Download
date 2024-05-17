encode_layoutreturn(struct xdr_stream *xdr,
		    const struct nfs4_layoutreturn_args *args,
		    struct compound_hdr *hdr)
{
	__be32 *p;

	p = reserve_space(xdr, 20);
	*p++ = cpu_to_be32(OP_LAYOUTRETURN);
	*p++ = cpu_to_be32(0);		 
	*p++ = cpu_to_be32(args->layout_type);
	*p++ = cpu_to_be32(IOMODE_ANY);
	*p = cpu_to_be32(RETURN_FILE);
	p = reserve_space(xdr, 16 + NFS4_STATEID_SIZE);
	p = xdr_encode_hyper(p, 0);
	p = xdr_encode_hyper(p, NFS4_MAX_UINT64);
	spin_lock(&args->inode->i_lock);
	xdr_encode_opaque_fixed(p, &args->stateid.data, NFS4_STATEID_SIZE);
	spin_unlock(&args->inode->i_lock);
	if (NFS_SERVER(args->inode)->pnfs_curr_ld->encode_layoutreturn) {
		NFS_SERVER(args->inode)->pnfs_curr_ld->encode_layoutreturn(
			NFS_I(args->inode)->layout, xdr, args);
	} else {
		p = reserve_space(xdr, 4);
		*p = cpu_to_be32(0);
	}
	hdr->nops++;
	hdr->replen += decode_layoutreturn_maxsz;
}