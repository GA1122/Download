sa_fini(sa_handle_t handle)
{
	sa_handle_impl_t impl_handle = (sa_handle_impl_t)handle;
	sa_share_impl_t impl_share, next;
	sa_share_impl_t *pcurr;

	if (impl_handle == NULL)
		return;

	 
	pcurr = &(impl_handle->shares);
	impl_share = *pcurr;
	while (impl_share != NULL) {
		next = impl_share->next;

		if (impl_share->dataset == NULL) {
			 
			*pcurr = next;

			sa_disable_share(impl_share, NULL);

			free_share(impl_share);
		} else {
			pcurr = &(impl_share->next);
		}

		impl_share = next;
	}

	update_sharetab(impl_handle);

	if (impl_handle->zfs_libhandle != NULL)
		libzfs_fini(impl_handle->zfs_libhandle);

	impl_share = impl_handle->shares;
	while (impl_share != NULL) {
		next = impl_share->next;
		free_share(impl_share);
		impl_share = next;
	}

	free(impl_handle);
}
