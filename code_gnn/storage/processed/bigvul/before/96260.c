find_share(sa_handle_impl_t impl_handle, const char *sharepath)
{
	sa_share_impl_t impl_share;

	impl_share = impl_handle->shares;
	while (impl_share != NULL) {
		if (strcmp(impl_share->sharepath, sharepath) == 0) {
			break;
		}

		impl_share = impl_share->next;
	}

	return (impl_share);
}
