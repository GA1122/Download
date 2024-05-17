int nfs4_init_ds_session(struct nfs_client *clp, unsigned long lease_time)
{
	struct nfs4_session *session = clp->cl_session;
	int ret;

	spin_lock(&clp->cl_lock);
	if (test_and_clear_bit(NFS4_SESSION_INITING, &session->session_state)) {
		 
		clp->cl_lease_time = lease_time;
		clp->cl_last_renewal = jiffies;
	}
	spin_unlock(&clp->cl_lock);

	ret = nfs41_check_session_ready(clp);
	if (ret)
		return ret;
	 
	if (!is_ds_client(clp))
		return -ENODEV;
	return 0;
}
