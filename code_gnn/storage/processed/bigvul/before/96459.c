static int expandRegular(rpmfi fi, const char *dest, rpmpsm psm, int exclusive, int nodigest, int nocontent)
{
    FD_t wfd = NULL;
    int rc = 0;

     
    {
	mode_t old_umask = umask(0577);
	wfd = Fopen(dest, exclusive ? "wx.ufdio" : "a.ufdio");
	umask(old_umask);

	 
	if (!exclusive && wfd != NULL && !linkSane(wfd, dest)) {
	    rc = RPMERR_OPEN_FAILED;
	    goto exit;
	}
    }
    if (Ferror(wfd)) {
	rc = RPMERR_OPEN_FAILED;
	goto exit;
    }

    if (!nocontent)
	rc = rpmfiArchiveReadToFilePsm(fi, wfd, nodigest, psm);
exit:
    if (wfd) {
	int myerrno = errno;
	static int oneshot = 0;
	static int flush_io = 0;
	if (!oneshot) {
	    flush_io = rpmExpandNumeric("%{?_flush_io}");
	    oneshot = 1;
	}
	if (flush_io) {
	    int fdno = Fileno(wfd);
	    fsync(fdno);
	}
	Fclose(wfd);
	errno = myerrno;
    }
    return rc;
}