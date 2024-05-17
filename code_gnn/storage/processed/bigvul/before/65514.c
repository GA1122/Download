nfs4_file_get_access(struct nfs4_file *fp, u32 access)
{
	lockdep_assert_held(&fp->fi_lock);

	 
	if (access & ~NFS4_SHARE_ACCESS_BOTH)
		return nfserr_inval;

	 
	if ((access & fp->fi_share_deny) != 0)
		return nfserr_share_denied;

	__nfs4_file_get_access(fp, access);
	return nfs_ok;
}