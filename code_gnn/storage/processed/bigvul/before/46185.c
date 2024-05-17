int freeze_super(struct super_block *sb)
{
	int ret;

	atomic_inc(&sb->s_active);
	down_write(&sb->s_umount);
	if (sb->s_writers.frozen != SB_UNFROZEN) {
		deactivate_locked_super(sb);
		return -EBUSY;
	}

	if (!(sb->s_flags & MS_BORN)) {
		up_write(&sb->s_umount);
		return 0;	 
	}

	if (sb->s_flags & MS_RDONLY) {
		 
		sb->s_writers.frozen = SB_FREEZE_COMPLETE;
		up_write(&sb->s_umount);
		return 0;
	}

	 
	sb->s_writers.frozen = SB_FREEZE_WRITE;
	smp_wmb();

	 
	up_write(&sb->s_umount);

	sb_wait_write(sb, SB_FREEZE_WRITE);

	 
	down_write(&sb->s_umount);
	sb->s_writers.frozen = SB_FREEZE_PAGEFAULT;
	smp_wmb();

	sb_wait_write(sb, SB_FREEZE_PAGEFAULT);

	 
	sync_filesystem(sb);

	 
	sb->s_writers.frozen = SB_FREEZE_FS;
	smp_wmb();
	sb_wait_write(sb, SB_FREEZE_FS);

	if (sb->s_op->freeze_fs) {
		ret = sb->s_op->freeze_fs(sb);
		if (ret) {
			printk(KERN_ERR
				"VFS:Filesystem freeze failed\n");
			sb->s_writers.frozen = SB_UNFROZEN;
			smp_wmb();
			wake_up(&sb->s_writers.wait_unfrozen);
			deactivate_locked_super(sb);
			return ret;
		}
	}
	 
	sb->s_writers.frozen = SB_FREEZE_COMPLETE;
	up_write(&sb->s_umount);
	return 0;
}
