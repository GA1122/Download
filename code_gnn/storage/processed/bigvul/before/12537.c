getHostCPS(struct rx_call *call, afs_uint32 ahost, prlist *alist,
	   afs_int32 *over)
{
    afs_int32 code, temp;
    struct ubik_trans *tt;
    struct prentry host_tentry;
    afs_int32 hostid;
    struct in_addr iaddr;
    char hoststr[16];

    *over = 0;
    iaddr.s_addr = ntohl(ahost);
    alist->prlist_len = 0;
    alist->prlist_val = NULL;
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

    code = NameToID(tt, afs_inet_ntoa_r(iaddr.s_addr, hoststr), &hostid);
    if (code == PRSUCCESS && hostid != 0) {
	temp = FindByID(tt, hostid);
	if (temp) {
	    code = pr_ReadEntry(tt, 0, temp, &host_tentry);
	    if (code == PRSUCCESS) {
		code = GetList(tt, &host_tentry, alist, 0);
		if (code)
		    goto bad;
	    } else
		fprintf(stderr, "pr_ReadEntry returned %d\n", code);
	} else
	    fprintf(stderr, "FindByID Failed -- Not found\n");
    }
    code = addWildCards(tt, alist, ntohl(ahost));
  bad:
    if (code != PRSUCCESS)
	ABORT_WITH(tt, code);

    code = ubik_EndTrans(tt);
    return code;
}
