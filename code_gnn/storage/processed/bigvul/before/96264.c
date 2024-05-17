process_share(sa_handle_impl_t impl_handle, sa_share_impl_t impl_share,
    char *pathname, char *resource, char *proto,
    char *options, char *description, char *dataset,
    boolean_t from_sharetab)
{
	struct stat statbuf;
	int rc;
	char *resource_dup = NULL, *dataset_dup = NULL;
	boolean_t new_share;
	sa_fstype_t *fstype;

	new_share = B_FALSE;

	if (impl_share == NULL)
		impl_share = find_share(impl_handle, pathname);

	if (impl_share == NULL) {
		if (lstat(pathname, &statbuf) != 0 ||
		    !S_ISDIR(statbuf.st_mode))
			return (SA_BAD_PATH);

		impl_share = alloc_share(pathname);

		if (impl_share == NULL) {
			rc = SA_NO_MEMORY;
			goto err;
		}

		new_share = B_TRUE;
	}

	if (dataset != NULL) {
		dataset_dup = strdup(dataset);

		if (dataset_dup == NULL) {
			rc = SA_NO_MEMORY;
			goto err;
		}
	}

	free(impl_share->dataset);
	impl_share->dataset = dataset_dup;

	rc = SA_INVALID_PROTOCOL;

	fstype = fstypes;
	while (fstype != NULL) {
		if (strcmp(fstype->name, proto) == 0) {
			if (resource != NULL) {
				resource_dup = strdup(resource);

				if (resource_dup == NULL) {
					rc = SA_NO_MEMORY;
					goto err;
				}
			}

			free(FSINFO(impl_share, fstype)->resource);
			FSINFO(impl_share, fstype)->resource = resource_dup;

			rc = fstype->ops->update_shareopts(impl_share,
			    resource, options);

			if (rc == SA_OK && from_sharetab)
				FSINFO(impl_share, fstype)->active = B_TRUE;

			break;
		}

		fstype = fstype->next;
	}

	if (rc != SA_OK)
		goto err;

	if (new_share) {
		impl_share->handle = impl_handle;

		impl_share->next = impl_handle->shares;
		impl_handle->shares = impl_share;

	}

err:
	if (rc != SA_OK) {
		if (new_share)
			free_share(impl_share);
	}

	return (rc);
}
