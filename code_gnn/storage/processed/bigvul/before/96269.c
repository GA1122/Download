sa_find_share(sa_handle_t handle, char *sharepath)
{
	return ((sa_share_t)find_share((sa_handle_impl_t)handle, sharepath));
}
