nfs_enable_share(sa_share_impl_t impl_share)
{
	char *shareopts, *linux_opts;
	int rc;

	if (!nfs_available()) {
		return (SA_SYSTEM_ERR);
	}

	shareopts = FSINFO(impl_share, nfs_fstype)->shareopts;

	if (shareopts == NULL)
		return (SA_OK);

	rc = get_linux_shareopts(shareopts, &linux_opts);

	if (rc != SA_OK)
		return (rc);

	rc = foreach_nfs_host(impl_share, nfs_enable_share_one, linux_opts);

	free(linux_opts);

	return (rc);
}
