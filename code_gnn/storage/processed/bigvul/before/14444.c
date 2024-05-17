DECL_PIOCTL(PGetUserCell)
{
    afs_int32 i;
    struct unixuser *tu;
    struct cell *tcell;

    AFS_STATCNT(PGetUserCell);
    if (!afs_resourceinit_flag)	 
	return EIO;		 

     
    i = UHash(areq->uid);
    ObtainWriteLock(&afs_xuser, 224);
    for (tu = afs_users[i]; tu; tu = tu->next) {
	if (tu->uid == areq->uid && (tu->states & UPrimary)) {
	    tu->refCount++;
	    ReleaseWriteLock(&afs_xuser);
	    break;
	}
    }
    if (tu) {
	tcell = afs_GetCell(tu->cell, READ_LOCK);
	afs_PutUser(tu, WRITE_LOCK);
	if (!tcell)
	    return ESRCH;
	else {
	    if (afs_pd_putString(aout, tcell->cellName) != 0)
		return E2BIG;
	    afs_PutCell(tcell, READ_LOCK);
	}
    } else {
	ReleaseWriteLock(&afs_xuser);
    }
    return 0;
}
