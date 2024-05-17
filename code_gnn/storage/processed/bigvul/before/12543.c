listEntries(struct rx_call *call, afs_int32 flag, afs_int32 startindex,
	    prentries *bulkentries, afs_int32 *nextstartindex, afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    afs_int32 i, eof, pos, maxentries, f;
    struct prentry tentry;
    afs_int32 pollcount = 0;

    *nextstartindex = -1;
    bulkentries->prentries_val = 0;
    bulkentries->prentries_len = 0;

    code = Initdb();
    if (code != PRSUCCESS)
	return code;
    code = ubik_BeginTransReadAny(dbase, UBIK_READTRANS, &tt);
    if (code)
	return code;
    code = ubik_SetLock(tt, 1, 1, LOCKREAD);
    if (code)
	ABORT_WITH(tt, code);
    code = read_DbHeader(tt);
    if (code)
	ABORT_WITH(tt, code);

     
    code = WhoIsThis(call, tt, cid);
    if (code)
	ABORT_WITH(tt, PRPERM);
    code = IsAMemberOf(tt, *cid, SYSADMINID);
    if (!code && !pr_noAuth)
	ABORT_WITH(tt, PRPERM);

    eof = ntohl(cheader.eofPtr) - sizeof(cheader);
    maxentries = eof / sizeof(struct prentry);
    for (i = startindex; i < maxentries; i++) {
	pos = i * sizeof(struct prentry) + sizeof(cheader);
	code = pr_ReadEntry(tt, 0, pos, &tentry);
	if (code)
	    goto done;

	if (++pollcount > 50) {
#ifndef AFS_PTHREAD_ENV
	    IOMGR_Poll();
#endif
	    pollcount = 0;
	}

	f = (tentry.flags & PRTYPE);
	if (((flag & PRUSERS) && (f == 0)) ||	 
	    ((flag & PRGROUPS) && (f & PRGRP))) {	 
	    code = put_prentries(&tentry, bulkentries);
	    if (code == -1)
		break;		 
	    if (code)
		goto done;
	}
    }
    code = 0;
    if (i < maxentries)
	*nextstartindex = i;

  done:
    if (code) {
	if (bulkentries->prentries_val)
	    free(bulkentries->prentries_val);
	bulkentries->prentries_val = 0;
	bulkentries->prentries_len = 0;
	ABORT_WITH(tt, code);
    } else {
	code = ubik_EndTrans(tt);
    }
    if (code)
	return code;
    return PRSUCCESS;
}
