DECL_PIOCTL(PStoreBehind)
{
    struct sbstruct sbr;

    if (afs_pd_getBytes(ain, &sbr, sizeof(struct sbstruct)) != 0)
	return EINVAL;

    if (sbr.sb_default != -1) {
	if (afs_osi_suser(*acred))
	    afs_defaultAsynchrony = sbr.sb_default;
	else
	    return EPERM;
    }

    if (avc && (sbr.sb_thisfile != -1)) {
	if (afs_AccessOK
	    (avc, PRSFS_WRITE | PRSFS_ADMINISTER, areq, DONT_CHECK_MODE_BITS))
	    avc->asynchrony = sbr.sb_thisfile;
	else
	    return EACCES;
    }

    memset(&sbr, 0, sizeof(sbr));
    sbr.sb_default = afs_defaultAsynchrony;
    if (avc) {
	sbr.sb_thisfile = avc->asynchrony;
    }

    return afs_pd_putBytes(aout, &sbr, sizeof(sbr));
}
