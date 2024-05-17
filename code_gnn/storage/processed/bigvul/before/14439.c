DECL_PIOCTL(PGetAcl)
{
    struct AFSOpaque acl;
    struct AFSVolSync tsync;
    struct AFSFetchStatus OutStatus;
    afs_int32 code;
    struct afs_conn *tconn;
    struct AFSFid Fid;
    struct rx_connection *rxconn;
    XSTATS_DECLS;

    AFS_STATCNT(PGetAcl);
    if (!avc)
	return EINVAL;
    Fid.Volume = avc->f.fid.Fid.Volume;
    Fid.Vnode = avc->f.fid.Fid.Vnode;
    Fid.Unique = avc->f.fid.Fid.Unique;
    if (avc->f.states & CForeign) {
	 
	if (Fid.Vnode & 0xc0000000)
	    return ERANGE;
	Fid.Vnode |= (ain->remaining << 30);
    }
    acl.AFSOpaque_val = aout->ptr;
    do {
	tconn = afs_Conn(&avc->f.fid, areq, SHARED_LOCK, &rxconn);
	if (tconn) {
	    acl.AFSOpaque_val[0] = '\0';
	    XSTATS_START_TIME(AFS_STATS_FS_RPCIDX_FETCHACL);
	    RX_AFS_GUNLOCK();
	    code = RXAFS_FetchACL(rxconn, &Fid, &acl, &OutStatus, &tsync);
	    RX_AFS_GLOCK();
	    XSTATS_END_TIME;
	} else
	    code = -1;
    } while (afs_Analyze
	     (tconn, rxconn, code, &avc->f.fid, areq, AFS_STATS_FS_RPCIDX_FETCHACL,
	      SHARED_LOCK, NULL));

    if (code == 0) {
	if (acl.AFSOpaque_len == 0)
	    afs_pd_skip(aout, 1);  
	else
	    afs_pd_skip(aout, acl.AFSOpaque_len);  
    }
    return code;
}
