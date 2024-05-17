dumpEntry(struct rx_call *call, afs_int32 apos, struct prdebugentry *aentry,
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

    code = WhoIsThis(call, tt, cid);
    if (code)
	ABORT_WITH(tt, PRPERM);
    code = pr_ReadEntry(tt, 0, apos, (struct prentry *)aentry);
    if (code)
	ABORT_WITH(tt, code);

    if (!AccessOK(tt, *cid, 0, PRP_STATUS_MEM, 0))
	ABORT_WITH(tt, PRPERM);

     
#if 0
    if (aentry->flags & PRCONT) {	 
	code = pr_ReadCoEntry(tt, 0, apos, aentry);
	if (code)
	    ABORT_WITH(tt, code);
    }
#endif
    code = ubik_EndTrans(tt);
    if (code)
	return code;
    return PRSUCCESS;
}
