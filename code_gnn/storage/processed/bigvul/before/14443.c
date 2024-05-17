DECL_PIOCTL(PGetWSCell)
{
    struct cell *tcell = NULL;

    AFS_STATCNT(PGetWSCell);
    if (!afs_resourceinit_flag)	 
	return EIO;		 

    tcell = afs_GetPrimaryCell(READ_LOCK);
    if (!tcell)			 
	return ESRCH;

    if (afs_pd_putString(aout, tcell->cellName) != 0)
	return EINVAL;
    afs_PutCell(tcell, READ_LOCK);
    return 0;
}
