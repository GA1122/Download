static int fsmRename(const char *opath, const char *path)
{
    removeSBITS(path);
    int rc = rename(opath, path);
#if defined(ETXTBSY) && defined(__HPUX__)
     
    if (rc && errno == ETXTBSY) {
	char *rmpath = NULL;
	rstrscat(&rmpath, path, "-RPMDELETE", NULL);
	rc = rename(path, rmpath);
	if (!rc) rc = rename(opath, path);
	free(rmpath);
    }
#endif
    if (_fsm_debug)
	rpmlog(RPMLOG_DEBUG, " %8s (%s, %s) %s\n", __func__,
	       opath, path, (rc < 0 ? strerror(errno) : ""));
    if (rc < 0)
	rc = (errno == EISDIR ? RPMERR_EXIST_AS_DIR : RPMERR_RENAME_FAILED);
    return rc;
}