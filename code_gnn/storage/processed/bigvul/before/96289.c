nfs_clear_shareopts(sa_share_impl_t impl_share)
{
	free(FSINFO(impl_share, nfs_fstype)->shareopts);
	FSINFO(impl_share, nfs_fstype)->shareopts = NULL;
}
