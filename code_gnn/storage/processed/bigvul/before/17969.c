kex_setup(struct ssh *ssh, char *proposal[PROPOSAL_MAX])
{
	int r;

	if ((r = kex_new(ssh, proposal, &ssh->kex)) != 0)
		return r;
	if ((r = kex_send_kexinit(ssh)) != 0) {		 
		kex_free(ssh->kex);
		ssh->kex = NULL;
		return r;
	}
	return 0;
}