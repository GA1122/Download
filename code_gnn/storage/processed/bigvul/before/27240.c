linux_to_osf_statfs(struct kstatfs *linux_stat, struct osf_statfs __user *osf_stat,
		    unsigned long bufsiz)
{
	struct osf_statfs tmp_stat;

	tmp_stat.f_type = linux_stat->f_type;
	tmp_stat.f_flags = 0;	 
	tmp_stat.f_fsize = linux_stat->f_frsize;
	tmp_stat.f_bsize = linux_stat->f_bsize;
	tmp_stat.f_blocks = linux_stat->f_blocks;
	tmp_stat.f_bfree = linux_stat->f_bfree;
	tmp_stat.f_bavail = linux_stat->f_bavail;
	tmp_stat.f_files = linux_stat->f_files;
	tmp_stat.f_ffree = linux_stat->f_ffree;
	tmp_stat.f_fsid = linux_stat->f_fsid;
	if (bufsiz > sizeof(tmp_stat))
		bufsiz = sizeof(tmp_stat);
	return copy_to_user(osf_stat, &tmp_stat, bufsiz) ? -EFAULT : 0;
}