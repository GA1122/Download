DECL_PIOCTL(PGCPAGs)
{
    if (!afs_osi_suser(*acred)) {
	return EACCES;
    }
    afs_gcpags = AFS_GCPAGS_USERDISABLED;
    return 0;
}
