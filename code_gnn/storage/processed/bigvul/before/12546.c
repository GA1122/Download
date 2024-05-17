listOwned(struct rx_call *call, afs_int32 aid, prlist *alist, afs_int32 *lastP,
	  afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;
    struct prentry tentry;
    afs_int32 head = 0;
    afs_int32 start;

    alist->prlist_len = 0;
    alist->prlist_val = NULL;

    if (!lastP)
	return PRBADARG;
    start = *lastP;
    *lastP = 0;

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

    if (start) {
	code = pr_ReadEntry(tt, 0, start, &tentry);
	if (!code && (tentry.owner == aid))
	    head = start;	 
    }

    if (!head) {
	if (aid) {
	    afs_int32 loc = FindByID(tt, aid);
	    if (loc == 0)
		ABORT_WITH(tt, PRNOENT);
	    code = pr_ReadEntry(tt, 0, loc, &tentry);
	    if (code)
		ABORT_WITH(tt, code);

	    if (!AccessOK(tt, *cid, &tentry, -1, PRP_OWNED_ANY))
		ABORT_WITH(tt, PRPERM);
	    head = tentry.owned;
	} else {
	    if (!AccessOK(tt, *cid, 0, 0, 0))
		ABORT_WITH(tt, PRPERM);
	    head = ntohl(cheader.orphan);
	}
    }

    code = GetOwnedChain(tt, &head, alist);
    if (code) {
	if (code == PRTOOMANY)
	    *lastP = head;
	else
	    ABORT_WITH(tt, code);
    }

    code = ubik_EndTrans(tt);
    return code;
}
