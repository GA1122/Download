static long btrfs_ioctl_qgroup_assign(struct file *file, void __user *arg)
{
	struct btrfs_root *root = BTRFS_I(fdentry(file)->d_inode)->root;
	struct btrfs_ioctl_qgroup_assign_args *sa;
	struct btrfs_trans_handle *trans;
	int ret;
	int err;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	ret = mnt_want_write_file(file);
	if (ret)
		return ret;

	sa = memdup_user(arg, sizeof(*sa));
	if (IS_ERR(sa)) {
		ret = PTR_ERR(sa);
		goto drop_write;
	}

	trans = btrfs_join_transaction(root);
	if (IS_ERR(trans)) {
		ret = PTR_ERR(trans);
		goto out;
	}

	 
	if (sa->assign) {
		ret = btrfs_add_qgroup_relation(trans, root->fs_info,
						sa->src, sa->dst);
	} else {
		ret = btrfs_del_qgroup_relation(trans, root->fs_info,
						sa->src, sa->dst);
	}

	err = btrfs_end_transaction(trans, root);
	if (err && !ret)
		ret = err;

out:
	kfree(sa);
drop_write:
	mnt_drop_write_file(file);
	return ret;
}