nfsd_vfs_write(struct svc_rqst *rqstp, struct svc_fh *fhp, struct file *file,
				loff_t offset, struct kvec *vec, int vlen,
				unsigned long *cnt, int stable)
{
	struct svc_export	*exp;
	mm_segment_t		oldfs;
	__be32			err = 0;
	int			host_err;
	int			use_wgather;
	loff_t			pos = offset;
	unsigned int		pflags = current->flags;
	int			flags = 0;

	if (test_bit(RQ_LOCAL, &rqstp->rq_flags))
		 
		current->flags |= PF_LESS_THROTTLE;

	exp = fhp->fh_export;
	use_wgather = (rqstp->rq_vers == 2) && EX_WGATHER(exp);

	if (!EX_ISSYNC(exp))
		stable = NFS_UNSTABLE;

	if (stable && !use_wgather)
		flags |= RWF_SYNC;

	 
	oldfs = get_fs(); set_fs(KERNEL_DS);
	host_err = vfs_writev(file, (struct iovec __user *)vec, vlen, &pos, flags);
	set_fs(oldfs);
	if (host_err < 0)
		goto out_nfserr;
	*cnt = host_err;
	nfsdstats.io_write += host_err;
	fsnotify_modify(file);

	if (stable && use_wgather)
		host_err = wait_for_concurrent_writes(file);

out_nfserr:
	dprintk("nfsd: write complete host_err=%d\n", host_err);
	if (host_err >= 0)
		err = 0;
	else
		err = nfserrno(host_err);
	if (test_bit(RQ_LOCAL, &rqstp->rq_flags))
		current_restore_flags(pflags, PF_LESS_THROTTLE);
	return err;
}