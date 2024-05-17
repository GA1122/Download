free_share(sa_share_impl_t impl_share) {
	sa_fstype_t *fstype;

	fstype = fstypes;
	while (fstype != NULL) {
		fstype->ops->clear_shareopts(impl_share);

		free(FSINFO(impl_share, fstype)->resource);

		fstype = fstype->next;
	}

	free(impl_share->sharepath);
	free(impl_share->dataset);
	free(impl_share->fsinfo);
	free(impl_share);
}
