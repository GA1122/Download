update_sharetab(sa_handle_impl_t impl_handle)
{
	sa_share_impl_t impl_share;
	int temp_fd;
	FILE *temp_fp;
	char tempfile[] = "/etc/dfs/sharetab.XXXXXX";
	sa_fstype_t *fstype;
	const char *resource;

	if (mkdir("/etc/dfs", 0755) < 0 && errno != EEXIST) {
		return;
	}

	temp_fd = mkstemp(tempfile);

	if (temp_fd < 0)
		return;

	temp_fp = fdopen(temp_fd, "w");

	if (temp_fp == NULL)
		return;

	impl_share = impl_handle->shares;
	while (impl_share != NULL) {
		fstype = fstypes;
		while (fstype != NULL) {
			if (FSINFO(impl_share, fstype)->active &&
			    FSINFO(impl_share, fstype)->shareopts != NULL) {
				resource = FSINFO(impl_share, fstype)->resource;

				if (resource == NULL)
					resource = "-";

				fprintf(temp_fp, "%s\t%s\t%s\t%s\n",
				    impl_share->sharepath, resource,
				    fstype->name,
				    FSINFO(impl_share, fstype)->shareopts);
			}

			fstype = fstype->next;
		}

		impl_share = impl_share->next;
	}

	fflush(temp_fp);
	fsync(temp_fd);
	fclose(temp_fp);

	rename(tempfile, "/etc/dfs/sharetab");
}
