static int fsmSetmeta(const char *path, rpmfi fi, rpmPlugins plugins,
		      rpmFileAction action, const struct stat * st,
		      int nofcaps)
{
    int rc = 0;
    const char *dest = rpmfiFN(fi);

    if (!rc && !getuid()) {
	rc = fsmChown(path, st->st_mode, st->st_uid, st->st_gid);
    }
    if (!rc && !S_ISLNK(st->st_mode)) {
	rc = fsmChmod(path, st->st_mode);
    }
     
    if (!rc && !nofcaps && S_ISREG(st->st_mode) && !getuid()) {
	rc = fsmSetFCaps(path, rpmfiFCaps(fi));
    }
    if (!rc) {
	rc = fsmUtime(path, st->st_mode, rpmfiFMtime(fi));
    }
    if (!rc) {
	rc = rpmpluginsCallFsmFilePrepare(plugins, fi,
					  path, dest, st->st_mode, action);
    }

    return rc;
}