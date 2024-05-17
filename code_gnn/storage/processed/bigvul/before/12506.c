CreateOK(struct ubik_trans *ut, afs_int32 cid, afs_int32 oid, afs_int32 flag,
	 int admin)
{
    if (restricted && !admin)
	return 0;

    if (flag & PRFOREIGN) {
	 
	return 0;
    } else if (flag & PRGRP) {
	 
	if (cid == ANONYMOUSID) {
	    if ((oid == 0) || !pr_noAuth)
		return 0;
	}
    } else {			 
	if (!admin && !pr_noAuth)
	    return 0;
    }
    return 1;			 
}
