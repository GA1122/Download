check_gss_callback_principal(struct nfs_client *clp, struct svc_rqst *rqstp)
{
	char *p = rqstp->rq_cred.cr_principal;

	if (rqstp->rq_authop->flavour != RPC_AUTH_GSS)
		return 1;

	 
	if (clp->cl_minorversion != 0)
		return 0;
	 
	if (p == NULL)
		return 0;

	 
	if (clp->cl_acceptor)
		return !strcmp(p, clp->cl_acceptor);

	 

	 

	if (memcmp(p, "nfs@", 4) != 0)
		return 0;
	p += 4;
	if (strcmp(p, clp->cl_hostname) != 0)
		return 0;
	return 1;
}
