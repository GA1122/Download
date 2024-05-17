static int md_ioctl(struct block_device *bdev, fmode_t mode,
			unsigned int cmd, unsigned long arg)
{
	int err = 0;
	void __user *argp = (void __user *)arg;
	struct mddev *mddev = NULL;
	int ro;

	if (!md_ioctl_valid(cmd))
		return -ENOTTY;

	switch (cmd) {
	case RAID_VERSION:
	case GET_ARRAY_INFO:
	case GET_DISK_INFO:
		break;
	default:
		if (!capable(CAP_SYS_ADMIN))
			return -EACCES;
	}

	 
	switch (cmd) {
	case RAID_VERSION:
		err = get_version(argp);
		goto out;

#ifndef MODULE
	case RAID_AUTORUN:
		err = 0;
		autostart_arrays(arg);
		goto out;
#endif
	default:;
	}

	 

	mddev = bdev->bd_disk->private_data;

	if (!mddev) {
		BUG();
		goto out;
	}

	 
	switch (cmd) {
	case GET_ARRAY_INFO:
		if (!mddev->raid_disks && !mddev->external)
			err = -ENODEV;
		else
			err = get_array_info(mddev, argp);
		goto out;

	case GET_DISK_INFO:
		if (!mddev->raid_disks && !mddev->external)
			err = -ENODEV;
		else
			err = get_disk_info(mddev, argp);
		goto out;

	case SET_DISK_FAULTY:
		err = set_disk_faulty(mddev, new_decode_dev(arg));
		goto out;

	case GET_BITMAP_FILE:
		err = get_bitmap_file(mddev, argp);
		goto out;

	}

	if (cmd == ADD_NEW_DISK)
		 
		flush_workqueue(md_misc_wq);

	if (cmd == HOT_REMOVE_DISK)
		 
		wait_event_interruptible_timeout(mddev->sb_wait,
						 !test_bit(MD_RECOVERY_NEEDED,
							   &mddev->flags),
						 msecs_to_jiffies(5000));
	if (cmd == STOP_ARRAY || cmd == STOP_ARRAY_RO) {
		 
		mutex_lock(&mddev->open_mutex);
		if (mddev->pers && atomic_read(&mddev->openers) > 1) {
			mutex_unlock(&mddev->open_mutex);
			err = -EBUSY;
			goto out;
		}
		set_bit(MD_STILL_CLOSED, &mddev->flags);
		mutex_unlock(&mddev->open_mutex);
		sync_blockdev(bdev);
	}
	err = mddev_lock(mddev);
	if (err) {
		printk(KERN_INFO
			"md: ioctl lock interrupted, reason %d, cmd %d\n",
			err, cmd);
		goto out;
	}

	if (cmd == SET_ARRAY_INFO) {
		mdu_array_info_t info;
		if (!arg)
			memset(&info, 0, sizeof(info));
		else if (copy_from_user(&info, argp, sizeof(info))) {
			err = -EFAULT;
			goto unlock;
		}
		if (mddev->pers) {
			err = update_array_info(mddev, &info);
			if (err) {
				printk(KERN_WARNING "md: couldn't update"
				       " array info. %d\n", err);
				goto unlock;
			}
			goto unlock;
		}
		if (!list_empty(&mddev->disks)) {
			printk(KERN_WARNING
			       "md: array %s already has disks!\n",
			       mdname(mddev));
			err = -EBUSY;
			goto unlock;
		}
		if (mddev->raid_disks) {
			printk(KERN_WARNING
			       "md: array %s already initialised!\n",
			       mdname(mddev));
			err = -EBUSY;
			goto unlock;
		}
		err = set_array_info(mddev, &info);
		if (err) {
			printk(KERN_WARNING "md: couldn't set"
			       " array info. %d\n", err);
			goto unlock;
		}
		goto unlock;
	}

	 
	 
	if ((!mddev->raid_disks && !mddev->external)
	    && cmd != ADD_NEW_DISK && cmd != STOP_ARRAY
	    && cmd != RUN_ARRAY && cmd != SET_BITMAP_FILE
	    && cmd != GET_BITMAP_FILE) {
		err = -ENODEV;
		goto unlock;
	}

	 
	switch (cmd) {
	case RESTART_ARRAY_RW:
		err = restart_array(mddev);
		goto unlock;

	case STOP_ARRAY:
		err = do_md_stop(mddev, 0, bdev);
		goto unlock;

	case STOP_ARRAY_RO:
		err = md_set_readonly(mddev, bdev);
		goto unlock;

	case HOT_REMOVE_DISK:
		err = hot_remove_disk(mddev, new_decode_dev(arg));
		goto unlock;

	case ADD_NEW_DISK:
		 
		if (mddev->pers) {
			mdu_disk_info_t info;
			if (copy_from_user(&info, argp, sizeof(info)))
				err = -EFAULT;
			else if (!(info.state & (1<<MD_DISK_SYNC)))
				 
				break;
			else
				err = add_new_disk(mddev, &info);
			goto unlock;
		}
		break;

	case BLKROSET:
		if (get_user(ro, (int __user *)(arg))) {
			err = -EFAULT;
			goto unlock;
		}
		err = -EINVAL;

		 
		if (ro)
			goto unlock;

		 
		if (mddev->ro != 1)
			goto unlock;

		 
		if (mddev->pers) {
			err = restart_array(mddev);
			if (err == 0) {
				mddev->ro = 2;
				set_disk_ro(mddev->gendisk, 0);
			}
		}
		goto unlock;
	}

	 
	if (mddev->ro && mddev->pers) {
		if (mddev->ro == 2) {
			mddev->ro = 0;
			sysfs_notify_dirent_safe(mddev->sysfs_state);
			set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
			 
			 
			if (test_bit(MD_CHANGE_DEVS, &mddev->flags)) {
				mddev_unlock(mddev);
				wait_event(mddev->sb_wait,
					   !test_bit(MD_CHANGE_DEVS, &mddev->flags) &&
					   !test_bit(MD_CHANGE_PENDING, &mddev->flags));
				mddev_lock_nointr(mddev);
			}
		} else {
			err = -EROFS;
			goto unlock;
		}
	}

	switch (cmd) {
	case ADD_NEW_DISK:
	{
		mdu_disk_info_t info;
		if (copy_from_user(&info, argp, sizeof(info)))
			err = -EFAULT;
		else
			err = add_new_disk(mddev, &info);
		goto unlock;
	}

	case CLUSTERED_DISK_NACK:
		if (mddev_is_clustered(mddev))
			md_cluster_ops->new_disk_ack(mddev, false);
		else
			err = -EINVAL;
		goto unlock;

	case HOT_ADD_DISK:
		err = hot_add_disk(mddev, new_decode_dev(arg));
		goto unlock;

	case RUN_ARRAY:
		err = do_md_run(mddev);
		goto unlock;

	case SET_BITMAP_FILE:
		err = set_bitmap_file(mddev, (int)arg);
		goto unlock;

	default:
		err = -EINVAL;
		goto unlock;
	}

unlock:
	if (mddev->hold_active == UNTIL_IOCTL &&
	    err != -EINVAL)
		mddev->hold_active = 0;
	mddev_unlock(mddev);
out:
	return err;
}
