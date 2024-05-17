static int nfs4_opendata_access(struct rpc_cred *cred,
				struct nfs4_opendata *opendata,
				struct nfs4_state *state, fmode_t fmode,
				int openflags)
{
	struct nfs_access_entry cache;
	u32 mask;

	 
	if (opendata->o_res.access_supported == 0)
		return 0;

	mask = 0;
	 
	if (openflags & __FMODE_EXEC) {
		 
		mask = MAY_EXEC;
	} else if ((fmode & FMODE_READ) && !opendata->file_created)
		mask = MAY_READ;

	cache.cred = cred;
	cache.jiffies = jiffies;
	nfs_access_set_mask(&cache, opendata->o_res.access_result);
	nfs_access_add_cache(state->inode, &cache);

	if ((mask & ~cache.mask & (MAY_READ | MAY_EXEC)) == 0)
		return 0;

	 
	nfs4_close_state(state, fmode);
	return -EACCES;
}
