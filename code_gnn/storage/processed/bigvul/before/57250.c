static bool nfs4_stateid_is_current(nfs4_stateid *stateid,
		const struct nfs_open_context *ctx,
		const struct nfs_lock_context *l_ctx,
		fmode_t fmode)
{
	nfs4_stateid current_stateid;

	 
	if (nfs4_set_rw_stateid(&current_stateid, ctx, l_ctx, fmode) == -EIO)
		return true;
	return nfs4_stateid_match(stateid, &current_stateid);
}