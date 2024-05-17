encode_wcc_data(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp)
{
	struct dentry	*dentry = fhp->fh_dentry;

	if (dentry && d_really_is_positive(dentry) && fhp->fh_post_saved) {
		if (fhp->fh_pre_saved) {
			*p++ = xdr_one;
			p = xdr_encode_hyper(p, (u64) fhp->fh_pre_size);
			p = encode_time3(p, &fhp->fh_pre_mtime);
			p = encode_time3(p, &fhp->fh_pre_ctime);
		} else {
			*p++ = xdr_zero;
		}
		return encode_saved_post_attr(rqstp, p, fhp);
	}
	 
	*p++ = xdr_zero;
	return encode_post_op_attr(rqstp, p, fhp);
}