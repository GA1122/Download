static bool nfsd4_cb_channel_good(struct nfs4_client *clp)
{
	if (clp->cl_cb_state == NFSD4_CB_UP)
		return true;
	 
	return clp->cl_minorversion && clp->cl_cb_state == NFSD4_CB_UNKNOWN;
}
