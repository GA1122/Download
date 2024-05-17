sa_disable_share(sa_share_t share, char *protocol)
{
	sa_share_impl_t impl_share = (sa_share_impl_t)share;
	int rc, ret;
	boolean_t found_protocol;
	sa_fstype_t *fstype;

#ifdef DEBUG
	fprintf(stderr, "sa_disable_share: share->sharepath=%s, protocol=%s\n",
		impl_share->sharepath, protocol);
#endif

	ret = SA_OK;
	found_protocol = B_FALSE;

	fstype = fstypes;
	while (fstype != NULL) {
		if (protocol == NULL || strcmp(fstype->name, protocol) == 0) {
			rc = fstype->ops->disable_share(impl_share);

			if (rc == SA_OK) {
				fstype->ops->clear_shareopts(impl_share);

				FSINFO(impl_share, fstype)->active = B_FALSE;
			} else
				ret = rc;

			found_protocol = B_TRUE;
		}

		fstype = fstype->next;
	}

	update_sharetab(impl_share->handle);

	return (found_protocol ? ret : SA_INVALID_PROTOCOL);
}
