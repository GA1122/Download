nfsd4_end_grace(struct nfsd_net *nn)
{
	 
	if (nn->grace_ended)
		return;

	dprintk("NFSD: end of grace period\n");
	nn->grace_ended = true;
	 
	nfsd4_record_grace_done(nn);
	 
	locks_end_grace(&nn->nfsd4_manager);
	 
}
