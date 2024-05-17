STALE_CLIENTID(clientid_t *clid, struct nfsd_net *nn)
{
	 
	if (clid->cl_boot == (u32)nn->boot_time)
		return 0;
	dprintk("NFSD stale clientid (%08x/%08x) boot_time %08lx\n",
		clid->cl_boot, clid->cl_id, nn->boot_time);
	return 1;
}
