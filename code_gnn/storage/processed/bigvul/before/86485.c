static int rpmVerifyScript(rpmts ts, Header h)
{
    int rc = 0;

    if (headerIsEntry(h, RPMTAG_VERIFYSCRIPT)) {
	 
	rpmte p = rpmteNew(ts, h, TR_REMOVED, NULL, NULL);

	if (p != NULL) {
	    rpmteSetHeader(p, h);

	    rc = (rpmpsmRun(ts, p, PKG_VERIFY) != RPMRC_OK);

	     
	    rpmteFree(p);
	} else {
	    rc = RPMRC_FAIL;
	}
    }

    return rc;
}
