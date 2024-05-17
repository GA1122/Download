update_zfs_share(sa_share_impl_t impl_share, const char *proto)
{
	sa_handle_impl_t impl_handle = impl_share->handle;
	zfs_handle_t *zhp;
	update_cookie_t udata;

	if (impl_handle->zfs_libhandle == NULL)
			return (SA_SYSTEM_ERR);

	assert(impl_share->dataset != NULL);

	zhp = zfs_open(impl_share->handle->zfs_libhandle, impl_share->dataset,
	    ZFS_TYPE_FILESYSTEM);

	if (zhp == NULL)
		return (SA_SYSTEM_ERR);

	udata.handle = impl_handle;
	udata.proto = proto;
	(void) update_zfs_shares_cb(zhp, &udata);

	return (SA_OK);
}
