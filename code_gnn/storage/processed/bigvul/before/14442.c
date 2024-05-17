DECL_PIOCTL(PGetFileCell)
{
    struct cell *tcell;

    AFS_STATCNT(PGetFileCell);
    if (!avc)
	return EINVAL;
    tcell = afs_GetCell(avc->f.fid.Cell, READ_LOCK);
    if (!tcell)
	return ESRCH;

    if (afs_pd_putString(aout, tcell->cellName) != 0)
	return EINVAL;

    afs_PutCell(tcell, READ_LOCK);
    return 0;
}
