nfsd4_putrootfh(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
		void *arg)
{
	__be32 status;

	fh_put(&cstate->current_fh);
	status = exp_pseudoroot(rqstp, &cstate->current_fh);
	return status;
}