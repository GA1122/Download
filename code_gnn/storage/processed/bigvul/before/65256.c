static __be32 *encode_entryplus_baggage(struct nfsd3_readdirres *cd, __be32 *p, const char *name, int namlen, u64 ino)
{
	struct svc_fh	*fh = &cd->scratch;
	__be32 err;

	fh_init(fh, NFS3_FHSIZE);
	err = compose_entry_fh(cd, fh, name, namlen, ino);
	if (err) {
		*p++ = 0;
		*p++ = 0;
		goto out;
	}
	p = encode_post_op_attr(cd->rqstp, p, fh);
	*p++ = xdr_one;			 
	p = encode_fh(p, fh);
out:
	fh_put(fh);
	return p;
}