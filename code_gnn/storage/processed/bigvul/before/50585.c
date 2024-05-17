static int ioctl_fsfreeze(struct file *filp)
{
	struct super_block *sb = file_inode(filp)->i_sb;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	 
	if (sb->s_op->freeze_fs == NULL && sb->s_op->freeze_super == NULL)
		return -EOPNOTSUPP;

	 
	if (sb->s_op->freeze_super)
		return sb->s_op->freeze_super(sb);
	return freeze_super(sb);
}