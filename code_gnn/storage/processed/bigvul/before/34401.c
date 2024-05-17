static long btrfs_ioctl_balance(struct file *file, void __user *arg)
{
	struct btrfs_root *root = BTRFS_I(fdentry(file)->d_inode)->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct btrfs_ioctl_balance_args *bargs;
	struct btrfs_balance_control *bctl;
	int ret;
	int need_to_clear_lock = 0;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	ret = mnt_want_write_file(file);
	if (ret)
		return ret;

	mutex_lock(&fs_info->volume_mutex);
	mutex_lock(&fs_info->balance_mutex);

	if (arg) {
		bargs = memdup_user(arg, sizeof(*bargs));
		if (IS_ERR(bargs)) {
			ret = PTR_ERR(bargs);
			goto out;
		}

		if (bargs->flags & BTRFS_BALANCE_RESUME) {
			if (!fs_info->balance_ctl) {
				ret = -ENOTCONN;
				goto out_bargs;
			}

			bctl = fs_info->balance_ctl;
			spin_lock(&fs_info->balance_lock);
			bctl->flags |= BTRFS_BALANCE_RESUME;
			spin_unlock(&fs_info->balance_lock);

			goto do_balance;
		}
	} else {
		bargs = NULL;
	}

	if (atomic_xchg(&root->fs_info->mutually_exclusive_operation_running,
			1)) {
		pr_info("btrfs: dev add/delete/balance/replace/resize operation in progress\n");
		ret = -EINPROGRESS;
		goto out_bargs;
	}
	need_to_clear_lock = 1;

	bctl = kzalloc(sizeof(*bctl), GFP_NOFS);
	if (!bctl) {
		ret = -ENOMEM;
		goto out_bargs;
	}

	bctl->fs_info = fs_info;
	if (arg) {
		memcpy(&bctl->data, &bargs->data, sizeof(bctl->data));
		memcpy(&bctl->meta, &bargs->meta, sizeof(bctl->meta));
		memcpy(&bctl->sys, &bargs->sys, sizeof(bctl->sys));

		bctl->flags = bargs->flags;
	} else {
		 
		bctl->flags |= BTRFS_BALANCE_TYPE_MASK;
	}

do_balance:
	ret = btrfs_balance(bctl, bargs);
	 
	if (arg) {
		if (copy_to_user(arg, bargs, sizeof(*bargs)))
			ret = -EFAULT;
	}

out_bargs:
	kfree(bargs);
out:
	if (need_to_clear_lock)
		atomic_set(&root->fs_info->mutually_exclusive_operation_running,
			   0);
	mutex_unlock(&fs_info->balance_mutex);
	mutex_unlock(&fs_info->volume_mutex);
	mnt_drop_write_file(file);
	return ret;
}