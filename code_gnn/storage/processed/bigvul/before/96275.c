sa_update_sharetab_ts(sa_handle_t handle)
{
	sa_handle_impl_t impl_handle = (sa_handle_impl_t)handle;

	update_sharetab(impl_handle);
}
