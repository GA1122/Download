encode_saved_post_attr(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp)
{
	 
	*p++ = xdr_one;
	return encode_fattr3(rqstp, p, fhp, &fhp->fh_post_attr);
}