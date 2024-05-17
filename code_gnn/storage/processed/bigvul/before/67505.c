int rpmPackageFilesRemove(rpmts ts, rpmte te, rpmfiles files,
              rpmpsm psm, char ** failedFile)
{
    rpmfi fi = rpmfilesIter(files, RPMFI_ITER_BACK);
    rpmfs fs = rpmteGetFileStates(te);
    rpmPlugins plugins = rpmtsPlugins(ts);
    struct stat sb;
    int rc = 0;
    char *fpath = NULL;

    while (!rc && rpmfiNext(fi) >= 0) {
	rpmFileAction action = rpmfsGetAction(fs, rpmfiFX(fi));
	fpath = fsmFsPath(fi, NULL);
	rc = fsmStat(fpath, 1, &sb);

	fsmDebug(fpath, action, &sb);

	 
	rc = rpmpluginsCallFsmFilePre(plugins, fi, fpath,
				      sb.st_mode, action);

	if (!XFA_SKIPPING(action))
	    rc = fsmBackup(fi, action);

         
        if (action == FA_ERASE) {
	    int missingok = (rpmfiFFlags(fi) & (RPMFILE_MISSINGOK | RPMFILE_GHOST));

	    rc = fsmRemove(fpath, sb.st_mode);

	     
	    if (rc == RPMERR_ENOENT && missingok) {
		rc = 0;
	    }

	     
	    if (rc == RPMERR_ENOTEMPTY) {
		rc = 0;
	    }

	    if (rc) {
		int lvl = strict_erasures ? RPMLOG_ERR : RPMLOG_WARNING;
		rpmlog(lvl, _("%s %s: remove failed: %s\n"),
			S_ISDIR(sb.st_mode) ? _("directory") : _("file"),
			fpath, strerror(errno));
            }
        }

	 
	rpmpluginsCallFsmFilePost(plugins, fi, fpath,
				  sb.st_mode, action, rc);

         
        if (!strict_erasures) rc = 0;

	if (rc)
	    *failedFile = xstrdup(fpath);

	if (rc == 0) {
	     
	     
	    rpm_loff_t amount = rpmfiFC(fi) - rpmfiFX(fi);
	    rpmpsmNotify(psm, RPMCALLBACK_UNINST_PROGRESS, amount);
	}
	fpath = _free(fpath);
    }

    free(fpath);
    rpmfiFree(fi);

    return rc;
}
