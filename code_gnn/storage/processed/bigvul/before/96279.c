update_zfs_shares(sa_handle_impl_t impl_handle, const char *proto)
{
	update_cookie_t udata;

	if (impl_handle->zfs_libhandle == NULL)
		return (SA_SYSTEM_ERR);

	udata.handle = impl_handle;
	udata.proto = proto;
	(void) zfs_iter_root(impl_handle->zfs_libhandle, update_zfs_shares_cb,
	    &udata);

	return (SA_OK);
}
