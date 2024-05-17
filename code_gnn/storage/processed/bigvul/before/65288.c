nfs3svc_encode_pathconfres(struct svc_rqst *rqstp, __be32 *p,
					struct nfsd3_pathconfres *resp)
{
	*p++ = xdr_zero;	 

	if (resp->status == 0) {
		*p++ = htonl(resp->p_link_max);
		*p++ = htonl(resp->p_name_max);
		*p++ = htonl(resp->p_no_trunc);
		*p++ = htonl(resp->p_chown_restricted);
		*p++ = htonl(resp->p_case_insensitive);
		*p++ = htonl(resp->p_case_preserving);
	}

	return xdr_ressize_check(rqstp, p);
}