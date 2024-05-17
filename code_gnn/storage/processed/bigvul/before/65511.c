__be32 nfs4_check_openmode(struct nfs4_ol_stateid *stp, int flags)
{
        __be32 status = nfserr_openmode;

	 
	if (stp->st_openstp)
		stp = stp->st_openstp;
	if ((flags & WR_STATE) && !access_permit_write(stp))
                goto out;
	if ((flags & RD_STATE) && !access_permit_read(stp))
                goto out;
	status = nfs_ok;
out:
	return status;
}