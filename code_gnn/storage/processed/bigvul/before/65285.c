nfs3svc_encode_fsinfores(struct svc_rqst *rqstp, __be32 *p,
					struct nfsd3_fsinfores *resp)
{
	*p++ = xdr_zero;	 

	if (resp->status == 0) {
		*p++ = htonl(resp->f_rtmax);
		*p++ = htonl(resp->f_rtpref);
		*p++ = htonl(resp->f_rtmult);
		*p++ = htonl(resp->f_wtmax);
		*p++ = htonl(resp->f_wtpref);
		*p++ = htonl(resp->f_wtmult);
		*p++ = htonl(resp->f_dtpref);
		p = xdr_encode_hyper(p, resp->f_maxfilesize);
		*p++ = xdr_one;
		*p++ = xdr_zero;
		*p++ = htonl(resp->f_properties);
	}

	return xdr_ressize_check(rqstp, p);
}