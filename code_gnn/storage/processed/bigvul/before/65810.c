nfsd4_encode_layoutreturn(struct nfsd4_compoundres *resp, __be32 nfserr,
		struct nfsd4_layoutreturn *lrp)
{
	struct xdr_stream *xdr = &resp->xdr;
	__be32 *p;

	if (nfserr)
		return nfserr;

	p = xdr_reserve_space(xdr, 4);
	if (!p)
		return nfserr_resource;
	*p++ = cpu_to_be32(lrp->lrs_present);
	if (lrp->lrs_present)
		return nfsd4_encode_stateid(xdr, &lrp->lr_sid);
	return nfs_ok;
}