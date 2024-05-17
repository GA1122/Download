static int nfsd_startup_generic(int nrservs)
{
	int ret;

	if (nfsd_users++)
		return 0;

	 
	ret = nfsd_racache_init(2*nrservs);
	if (ret)
		goto dec_users;

	ret = nfs4_state_start();
	if (ret)
		goto out_racache;
	return 0;

out_racache:
	nfsd_racache_shutdown();
dec_users:
	nfsd_users--;
	return ret;
}
