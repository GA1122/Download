DECL_PIOCTL(PSetAcl)
{
    afs_int32 code;
    struct afs_conn *tconn;
    struct AFSOpaque acl;
    struct AFSVolSync tsync;
    struct AFSFetchStatus OutStatus;
    struct rx_connection *rxconn;
    XSTATS_DECLS;

    AFS_STATCNT(PSetAcl);
    if (!avc)
	return EINVAL;

    if (afs_pd_getStringPtr(ain, &acl.AFSOpaque_val) != 0)
	return EINVAL;
    acl.AFSOpaque_len = strlen(acl.AFSOpaque_val) + 1;
    if (acl.AFSOpaque_len > 1024)
	return EINVAL;

    do {
	tconn = afs_Conn(&avc->f.fid, areq, SHARED_LOCK, &rxconn);
	if (tconn) {
	    XSTATS_START_TIME(AFS_STATS_FS_RPCIDX_STOREACL);
	    RX_AFS_GUNLOCK();
	    code =
		RXAFS_StoreACL(rxconn, (struct AFSFid *)&avc->f.fid.Fid,
			       &acl, &OutStatus, &tsync);
	    RX_AFS_GLOCK();
	    XSTATS_END_TIME;
	} else
	    code = -1;
    } while (afs_Analyze
	     (tconn, rxconn, code, &avc->f.fid, areq, AFS_STATS_FS_RPCIDX_STOREACL,
	      SHARED_LOCK, NULL));

     
    ObtainWriteLock(&afs_xcbhash, 455);
    avc->callback = 0;
    afs_DequeueCallback(avc);
    avc->f.states &= ~(CStatd | CUnique);
    ReleaseWriteLock(&afs_xcbhash);
    if (avc->f.fid.Fid.Vnode & 1 || (vType(avc) == VDIR))
	osi_dnlc_purgedp(avc);

     
    return code;
}
