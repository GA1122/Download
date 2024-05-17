encode_layoutcommit(struct xdr_stream *xdr,
		    struct inode *inode,
		    const struct nfs4_layoutcommit_args *args,
		    struct compound_hdr *hdr)
{
	__be32 *p;

	dprintk("%s: lbw: %llu type: %d\n", __func__, args->lastbytewritten,
		NFS_SERVER(args->inode)->pnfs_curr_ld->id);

	p = reserve_space(xdr, 44 + NFS4_STATEID_SIZE);
	*p++ = cpu_to_be32(OP_LAYOUTCOMMIT);
	 
	p = xdr_encode_hyper(p, 0);  
	p = xdr_encode_hyper(p, args->lastbytewritten + 1);	 
	*p++ = cpu_to_be32(0);  
	p = xdr_encode_opaque_fixed(p, args->stateid.data, NFS4_STATEID_SIZE);
	*p++ = cpu_to_be32(1);  
	p = xdr_encode_hyper(p, args->lastbytewritten);
	*p++ = cpu_to_be32(0);  
	*p++ = cpu_to_be32(NFS_SERVER(args->inode)->pnfs_curr_ld->id); 

	if (NFS_SERVER(inode)->pnfs_curr_ld->encode_layoutcommit)
		NFS_SERVER(inode)->pnfs_curr_ld->encode_layoutcommit(
			NFS_I(inode)->layout, xdr, args);
	else {
		p = reserve_space(xdr, 4);
		*p = cpu_to_be32(0);  
	}

	hdr->nops++;
	hdr->replen += decode_layoutcommit_maxsz;
	return 0;
}
