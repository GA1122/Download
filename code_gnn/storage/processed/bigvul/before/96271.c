sa_get_zfs_handle(sa_handle_t handle)
{
	sa_handle_impl_t impl_handle = (sa_handle_impl_t)handle;

	if (impl_handle == NULL)
		return (NULL);

	return (impl_handle->zfs_libhandle);
}
