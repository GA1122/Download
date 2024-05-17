isAMemberOf(struct rx_call *call, afs_int32 uid, afs_int32 gid, afs_int32 *flag,
	    afs_int32 *cid)
{
    afs_int32 code;
    struct ubik_trans *tt;

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

    {
	afs_int32 uloc = FindByID(tt, uid);
	afs_int32 gloc = FindByID(tt, gid);
	struct prentry uentry, gentry;

	if (!uloc || !gloc)
	    ABORT_WITH(tt, PRNOENT);
	code = WhoIsThis(call, tt, cid);
	if (code)
	    ABORT_WITH(tt, PRPERM);
	code = pr_ReadEntry(tt, 0, uloc, &uentry);
	if (code)
	    ABORT_WITH(tt, code);
	code = pr_ReadEntry(tt, 0, gloc, &gentry);
	if (code)
	    ABORT_WITH(tt, code);
#if !defined(SUPERGROUPS)
	if ((uentry.flags & PRGRP) || !(gentry.flags & PRGRP))
	    ABORT_WITH(tt, PRBADARG);
#else
	if (!(gentry.flags & PRGRP))
	    ABORT_WITH(tt, PRBADARG);
#endif
	if (!AccessOK(tt, *cid, &uentry, 0, PRP_MEMBER_ANY)
	    && !AccessOK(tt, *cid, &gentry, PRP_MEMBER_MEM, PRP_MEMBER_ANY))
	    ABORT_WITH(tt, PRPERM);
    }

    *flag = IsAMemberOf(tt, uid, gid);
    code = ubik_EndTrans(tt);
    return code;
}
