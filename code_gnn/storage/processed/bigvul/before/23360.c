encode_getdeviceinfo(struct xdr_stream *xdr,
		     const struct nfs4_getdeviceinfo_args *args,
		     struct compound_hdr *hdr)
{
	__be32 *p;

	p = reserve_space(xdr, 16 + NFS4_DEVICEID4_SIZE);
	*p++ = cpu_to_be32(OP_GETDEVICEINFO);
	p = xdr_encode_opaque_fixed(p, args->pdev->dev_id.data,
				    NFS4_DEVICEID4_SIZE);
	*p++ = cpu_to_be32(args->pdev->layout_type);
	*p++ = cpu_to_be32(args->pdev->pglen);		 
	*p++ = cpu_to_be32(0);				 
	hdr->nops++;
	hdr->replen += decode_getdeviceinfo_maxsz;
}