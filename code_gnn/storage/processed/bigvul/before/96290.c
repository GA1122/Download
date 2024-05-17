nfs_disable_share(sa_share_impl_t impl_share)
{
	if (!nfs_available()) {
		 
		return (SA_OK);
	}

	return (foreach_nfs_host(impl_share, nfs_disable_share_one, NULL));
}
