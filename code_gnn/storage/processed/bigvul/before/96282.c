foreach_nfs_host(sa_share_impl_t impl_share, nfs_host_callback_t callback,
    void *cookie)
{
	nfs_host_cookie_t udata;
	char *shareopts;

	udata.callback = callback;
	udata.sharepath = impl_share->sharepath;
	udata.cookie = cookie;
	udata.security = "sys";

	shareopts = FSINFO(impl_share, nfs_fstype)->shareopts;

	return foreach_nfs_shareopt(shareopts, foreach_nfs_host_cb,
	    &udata);
}
