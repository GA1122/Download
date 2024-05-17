iNewEntry(struct rx_call *call, char aname[], afs_int32 aid, afs_int32 oid,
	  afs_int32 *cid)
{
     
    struct ubik_trans *tt;
    afs_int32 code;
    afs_int32 gflag = 0;
    int admin;

    stolower(aname);
    code = Initdb();
    if (code != PRSUCCESS)
	return code;
    code = ubik_BeginTrans(dbase, UBIK_WRITETRANS, &tt);
    if (code)
	return code;
    code = ubik_SetLock(tt, 1, 1, LOCKWRITE);
    if (code)
	ABORT_WITH(tt, code);
    code = read_DbHeader(tt);
    if (code)
	ABORT_WITH(tt, code);

    code = WhoIsThis(call, tt, cid);
    if (code)
	ABORT_WITH(tt, PRPERM);
    admin = IsAMemberOf(tt, *cid, SYSADMINID);

     
    if (aid == 0)
	ABORT_WITH(tt, PRPERM);
    if (aid < 0) {
	gflag |= PRGRP;
	 
	if (!admin && !pr_noAuth && (aid != ntohl(cheader.maxGroup) - 1))
	    ABORT_WITH(tt, PRPERM);
    }
    if (FindByID(tt, aid))
	ABORT_WITH(tt, PRIDEXIST);

     
    if (!CreateOK(tt, *cid, oid, gflag, admin))
	ABORT_WITH(tt, PRPERM);

    code = CreateEntry(tt, aname, &aid, 1, gflag, oid, *cid);
    if (code != PRSUCCESS)
	ABORT_WITH(tt, code);

     
    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
