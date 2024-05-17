derelativise_path(const char *path)
{
	char *expanded, *ret, cwd[PATH_MAX];

	if (strcasecmp(path, "none") == 0)
		return xstrdup("none");
	expanded = tilde_expand_filename(path, getuid());
	if (*expanded == '/')
		return expanded;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		fatal("%s: getcwd: %s", __func__, strerror(errno));
	xasprintf(&ret, "%s/%s", cwd, expanded);
	free(expanded);
	return ret;
}
