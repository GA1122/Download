sa_zfs_process_share(sa_handle_t handle, sa_group_t group, sa_share_t share,
    char *mountpoint, char *proto, zprop_source_t source, char *shareopts,
    char *sourcestr, char *dataset)
{
	sa_handle_impl_t impl_handle = (sa_handle_impl_t)handle;
	sa_share_impl_t impl_share = (sa_share_impl_t)share;

#ifdef DEBUG
	fprintf(stderr, "sa_zfs_process_share: mountpoint=%s, proto=%s, "
	    "shareopts=%s, sourcestr=%s, dataset=%s\n", mountpoint, proto,
	    shareopts, sourcestr, dataset);
#endif

	return (process_share(impl_handle, impl_share, mountpoint, NULL,
	    proto, shareopts, NULL, dataset, B_FALSE));
}
