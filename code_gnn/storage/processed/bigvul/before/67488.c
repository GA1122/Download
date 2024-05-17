static int fsmCommit(char **path, rpmfi fi, rpmFileAction action, const char *suffix)
{
    int rc = 0;

     
    if (!(S_ISSOCK(rpmfiFMode(fi)) && IS_DEV_LOG(*path))) {
	const char *nsuffix = (action == FA_ALTNAME) ? SUFFIX_RPMNEW : NULL;
	char *dest = *path;
	 
	if (suffix)
	    dest = fsmFsPath(fi, nsuffix);

	 
	if (dest != *path) {
	    rc = fsmRename(*path, dest);
	    if (!rc && nsuffix) {
		char * opath = fsmFsPath(fi, NULL);
		rpmlog(RPMLOG_WARNING, _("%s created as %s\n"),
		       opath, dest);
		free(opath);
	    }
	    free(*path);
	    *path = dest;
	}
    }

    return rc;
}
