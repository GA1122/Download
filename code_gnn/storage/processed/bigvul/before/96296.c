nfs_validate_shareopts(const char *shareopts)
{
	char *linux_opts;
	int rc;

	rc = get_linux_shareopts(shareopts, &linux_opts);

	if (rc != SA_OK)
		return (rc);

	free(linux_opts);

	return (SA_OK);
}
