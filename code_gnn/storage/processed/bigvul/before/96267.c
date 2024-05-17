sa_enable_share(sa_share_t share, char *protocol)
{
	sa_share_impl_t impl_share = (sa_share_impl_t)share;
	int rc, ret;
	boolean_t found_protocol;
	sa_fstype_t *fstype;

#ifdef DEBUG
	fprintf(stderr, "sa_enable_share: share->sharepath=%s, protocol=%s\n",
		impl_share->sharepath, protocol);
#endif

	assert(impl_share->handle != NULL);

	ret = SA_OK;
	found_protocol = B_FALSE;

	fstype = fstypes;
	while (fstype != NULL) {
		if (protocol == NULL || strcmp(fstype->name, protocol) == 0) {
			update_zfs_share(impl_share, fstype->name);

			rc = fstype->ops->enable_share(impl_share);

			if (rc != SA_OK)
				ret = rc;
			else
				FSINFO(impl_share, fstype)->active = B_TRUE;

			found_protocol = B_TRUE;
		}

		fstype = fstype->next;
	}

	update_sharetab(impl_share->handle);

	return (found_protocol ? ret : SA_INVALID_PROTOCOL);
}
