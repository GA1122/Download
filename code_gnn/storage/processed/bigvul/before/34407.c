static int btrfs_ioctl_defrag(struct file *file, void __user *argp)
{
	struct inode *inode = fdentry(file)->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_ioctl_defrag_range_args *range;
	int ret;

	if (btrfs_root_readonly(root))
		return -EROFS;

	if (atomic_xchg(&root->fs_info->mutually_exclusive_operation_running,
			1)) {
		pr_info("btrfs: dev add/delete/balance/replace/resize operation in progress\n");
		return -EINPROGRESS;
	}
	ret = mnt_want_write_file(file);
	if (ret) {
		atomic_set(&root->fs_info->mutually_exclusive_operation_running,
			   0);
		return ret;
	}

	switch (inode->i_mode & S_IFMT) {
	case S_IFDIR:
		if (!capable(CAP_SYS_ADMIN)) {
			ret = -EPERM;
			goto out;
		}
		ret = btrfs_defrag_root(root, 0);
		if (ret)
			goto out;
		ret = btrfs_defrag_root(root->fs_info->extent_root, 0);
		break;
	case S_IFREG:
		if (!(file->f_mode & FMODE_WRITE)) {
			ret = -EINVAL;
			goto out;
		}

		range = kzalloc(sizeof(*range), GFP_KERNEL);
		if (!range) {
			ret = -ENOMEM;
			goto out;
		}

		if (argp) {
			if (copy_from_user(range, argp,
					   sizeof(*range))) {
				ret = -EFAULT;
				kfree(range);
				goto out;
			}
			 
			if ((range->flags & BTRFS_DEFRAG_RANGE_COMPRESS)) {
				range->flags |= BTRFS_DEFRAG_RANGE_START_IO;
				range->extent_thresh = (u32)-1;
			}
		} else {
			 
			range->len = (u64)-1;
		}
		ret = btrfs_defrag_file(fdentry(file)->d_inode, file,
					range, 0, 0);
		if (ret > 0)
			ret = 0;
		kfree(range);
		break;
	default:
		ret = -EINVAL;
	}
out:
	mnt_drop_write_file(file);
	atomic_set(&root->fs_info->mutually_exclusive_operation_running, 0);
	return ret;
}