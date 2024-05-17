alloc_share(const char *sharepath)
{
	sa_share_impl_t impl_share;

	impl_share = calloc(sizeof (struct sa_share_impl), 1);

	if (impl_share == NULL)
		return (NULL);

	impl_share->sharepath = strdup(sharepath);

	if (impl_share->sharepath == NULL) {
		free(impl_share);
		return (NULL);
	}

	impl_share->fsinfo = calloc(sizeof (sa_share_fsinfo_t), fstypes_count);

	if (impl_share->fsinfo == NULL) {
		free(impl_share->sharepath);
		free(impl_share);
		return (NULL);
	}

	return (impl_share);
}
