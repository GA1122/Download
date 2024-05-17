static void cull_mntent_opt(struct mntent *mntent)
{
	int i;
	char *p, *p2;
	char *list[] = {"create=dir",
			"create=file",
			"optional",
			NULL };

	for (i=0; list[i]; i++) {
		if (!(p = strstr(mntent->mnt_opts, list[i])))
			continue;
		p2 = strchr(p, ',');
		if (!p2) {
			 
			*p = '\0';
			continue;
		}
		memmove(p, p2+1, strlen(p2+1)+1);
	}
}