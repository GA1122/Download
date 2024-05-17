DECL_PIOCTL(PGetFID)
{
    AFS_STATCNT(PGetFID);
    if (!avc)
	return EINVAL;
    if (afs_pd_putBytes(aout, &avc->f.fid, sizeof(struct VenusFid)) != 0)
	return EINVAL;
    return 0;
}
