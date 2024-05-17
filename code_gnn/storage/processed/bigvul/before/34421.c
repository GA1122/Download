static noinline int btrfs_ioctl_resize(struct file *file,
					void __user *arg)
{
	u64 new_size;
	u64 old_size;
	u64 devid = 1;
	struct btrfs_root *root = BTRFS_I(fdentry(file)->d_inode)->root;
	struct btrfs_ioctl_vol_args *vol_args;
	struct btrfs_trans_handle *trans;
	struct btrfs_device *device = NULL;
	char *sizestr;
	char *devstr = NULL;
	int ret = 0;
	int mod = 0;

	if (root->fs_info->sb->s_flags & MS_RDONLY)
		return -EROFS;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	ret = mnt_want_write_file(file);
	if (ret)
		return ret;

	if (atomic_xchg(&root->fs_info->mutually_exclusive_operation_running,
			1)) {
		pr_info("btrfs: dev add/delete/balance/replace/resize operation in progress\n");
		return -EINPROGRESS;
	}

	mutex_lock(&root->fs_info->volume_mutex);
	vol_args = memdup_user(arg, sizeof(*vol_args));
	if (IS_ERR(vol_args)) {
		ret = PTR_ERR(vol_args);
		goto out;
	}

	vol_args->name[BTRFS_PATH_NAME_MAX] = '\0';

	sizestr = vol_args->name;
	devstr = strchr(sizestr, ':');
	if (devstr) {
		char *end;
		sizestr = devstr + 1;
		*devstr = '\0';
		devstr = vol_args->name;
		devid = simple_strtoull(devstr, &end, 10);
		printk(KERN_INFO "btrfs: resizing devid %llu\n",
		       (unsigned long long)devid);
	}
	device = btrfs_find_device(root->fs_info, devid, NULL, NULL);
	if (!device) {
		printk(KERN_INFO "btrfs: resizer unable to find device %llu\n",
		       (unsigned long long)devid);
		ret = -EINVAL;
		goto out_free;
	}
	if (device->fs_devices && device->fs_devices->seeding) {
		printk(KERN_INFO "btrfs: resizer unable to apply on "
		       "seeding device %llu\n",
		       (unsigned long long)devid);
		ret = -EINVAL;
		goto out_free;
	}

	if (!strcmp(sizestr, "max"))
		new_size = device->bdev->bd_inode->i_size;
	else {
		if (sizestr[0] == '-') {
			mod = -1;
			sizestr++;
		} else if (sizestr[0] == '+') {
			mod = 1;
			sizestr++;
		}
		new_size = memparse(sizestr, NULL);
		if (new_size == 0) {
			ret = -EINVAL;
			goto out_free;
		}
	}

	if (device->is_tgtdev_for_dev_replace) {
		ret = -EINVAL;
		goto out_free;
	}

	old_size = device->total_bytes;

	if (mod < 0) {
		if (new_size > old_size) {
			ret = -EINVAL;
			goto out_free;
		}
		new_size = old_size - new_size;
	} else if (mod > 0) {
		new_size = old_size + new_size;
	}

	if (new_size < 256 * 1024 * 1024) {
		ret = -EINVAL;
		goto out_free;
	}
	if (new_size > device->bdev->bd_inode->i_size) {
		ret = -EFBIG;
		goto out_free;
	}

	do_div(new_size, root->sectorsize);
	new_size *= root->sectorsize;

	printk_in_rcu(KERN_INFO "btrfs: new size for %s is %llu\n",
		      rcu_str_deref(device->name),
		      (unsigned long long)new_size);

	if (new_size > old_size) {
		trans = btrfs_start_transaction(root, 0);
		if (IS_ERR(trans)) {
			ret = PTR_ERR(trans);
			goto out_free;
		}
		ret = btrfs_grow_device(trans, device, new_size);
		btrfs_commit_transaction(trans, root);
	} else if (new_size < old_size) {
		ret = btrfs_shrink_device(device, new_size);
	}  

out_free:
	kfree(vol_args);
out:
	mutex_unlock(&root->fs_info->volume_mutex);
	mnt_drop_write_file(file);
	atomic_set(&root->fs_info->mutually_exclusive_operation_running, 0);
	return ret;
}