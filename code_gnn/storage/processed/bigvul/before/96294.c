nfs_is_share_active(sa_share_impl_t impl_share)
{
	char line[512];
	char *tab, *cur;
	FILE *nfs_exportfs_temp_fp;

	if (!nfs_available())
		return (B_FALSE);

	nfs_exportfs_temp_fp = fdopen(dup(nfs_exportfs_temp_fd), "r");

	if (nfs_exportfs_temp_fp == NULL ||
	    fseek(nfs_exportfs_temp_fp, 0, SEEK_SET) < 0) {
		fclose(nfs_exportfs_temp_fp);
		return (B_FALSE);
	}

	while (fgets(line, sizeof (line), nfs_exportfs_temp_fp) != NULL) {
		 
		if (line[0] == '\t')
			continue;

		tab = strchr(line, '\t');

		if (tab != NULL) {
			*tab = '\0';
			cur = tab - 1;
		} else {
			 
			cur = line + strlen(line) - 1;
		}

		 
		while (cur >= line && (*cur == ' ' || *cur == '\n'))
			*cur-- = '\0';

		if (strcmp(line, impl_share->sharepath) == 0) {
			fclose(nfs_exportfs_temp_fp);
			return (B_TRUE);
		}
	}

	fclose(nfs_exportfs_temp_fp);

	return (B_FALSE);
}
