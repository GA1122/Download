void md_do_sync(struct md_thread *thread)
{
	struct mddev *mddev = thread->mddev;
	struct mddev *mddev2;
	unsigned int currspeed = 0,
		 window;
	sector_t max_sectors,j, io_sectors, recovery_done;
	unsigned long mark[SYNC_MARKS];
	unsigned long update_time;
	sector_t mark_cnt[SYNC_MARKS];
	int last_mark,m;
	struct list_head *tmp;
	sector_t last_check;
	int skipped = 0;
	struct md_rdev *rdev;
	char *desc, *action = NULL;
	struct blk_plug plug;

	 
	if (test_bit(MD_RECOVERY_DONE, &mddev->recovery))
		return;
	if (mddev->ro) { 
		set_bit(MD_RECOVERY_INTR, &mddev->recovery);
		return;
	}

	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
		if (test_bit(MD_RECOVERY_CHECK, &mddev->recovery)) {
			desc = "data-check";
			action = "check";
		} else if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery)) {
			desc = "requested-resync";
			action = "repair";
		} else
			desc = "resync";
	} else if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery))
		desc = "reshape";
	else
		desc = "recovery";

	mddev->last_sync_action = action ?: desc;

	 

	do {
		mddev->curr_resync = 2;

	try_again:
		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			goto skip;
		for_each_mddev(mddev2, tmp) {
			if (mddev2 == mddev)
				continue;
			if (!mddev->parallel_resync
			&&  mddev2->curr_resync
			&&  match_mddev_units(mddev, mddev2)) {
				DEFINE_WAIT(wq);
				if (mddev < mddev2 && mddev->curr_resync == 2) {
					 
					mddev->curr_resync = 1;
					wake_up(&resync_wait);
				}
				if (mddev > mddev2 && mddev->curr_resync == 1)
					 
					continue;
				 
				prepare_to_wait(&resync_wait, &wq, TASK_INTERRUPTIBLE);
				if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery) &&
				    mddev2->curr_resync >= mddev->curr_resync) {
					printk(KERN_INFO "md: delaying %s of %s"
					       " until %s has finished (they"
					       " share one or more physical units)\n",
					       desc, mdname(mddev), mdname(mddev2));
					mddev_put(mddev2);
					if (signal_pending(current))
						flush_signals(current);
					schedule();
					finish_wait(&resync_wait, &wq);
					goto try_again;
				}
				finish_wait(&resync_wait, &wq);
			}
		}
	} while (mddev->curr_resync < 2);

	j = 0;
	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
		 
		max_sectors = mddev->resync_max_sectors;
		atomic64_set(&mddev->resync_mismatches, 0);
		 
		if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
			j = mddev->resync_min;
		else if (!mddev->bitmap)
			j = mddev->recovery_cp;

	} else if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery))
		max_sectors = mddev->resync_max_sectors;
	else {
		 
		max_sectors = mddev->dev_sectors;
		j = MaxSector;
		rcu_read_lock();
		rdev_for_each_rcu(rdev, mddev)
			if (rdev->raid_disk >= 0 &&
			    !test_bit(Faulty, &rdev->flags) &&
			    !test_bit(In_sync, &rdev->flags) &&
			    rdev->recovery_offset < j)
				j = rdev->recovery_offset;
		rcu_read_unlock();

		 
		if (mddev->bitmap) {
			mddev->pers->quiesce(mddev, 1);
			mddev->pers->quiesce(mddev, 0);
		}
	}

	printk(KERN_INFO "md: %s of RAID array %s\n", desc, mdname(mddev));
	printk(KERN_INFO "md: minimum _guaranteed_  speed:"
		" %d KB/sec/disk.\n", speed_min(mddev));
	printk(KERN_INFO "md: using maximum available idle IO bandwidth "
	       "(but not more than %d KB/sec) for %s.\n",
	       speed_max(mddev), desc);

	is_mddev_idle(mddev, 1);  

	io_sectors = 0;
	for (m = 0; m < SYNC_MARKS; m++) {
		mark[m] = jiffies;
		mark_cnt[m] = io_sectors;
	}
	last_mark = 0;
	mddev->resync_mark = mark[last_mark];
	mddev->resync_mark_cnt = mark_cnt[last_mark];

	 
	window = 32*(PAGE_SIZE/512);
	printk(KERN_INFO "md: using %dk window, over a total of %lluk.\n",
		window/2, (unsigned long long)max_sectors/2);

	atomic_set(&mddev->recovery_active, 0);
	last_check = 0;

	if (j>2) {
		printk(KERN_INFO
		       "md: resuming %s of %s from checkpoint.\n",
		       desc, mdname(mddev));
		mddev->curr_resync = j;
	} else
		mddev->curr_resync = 3;  
	mddev->curr_resync_completed = j;
	sysfs_notify(&mddev->kobj, NULL, "sync_completed");
	md_new_event(mddev);
	update_time = jiffies;

	if (mddev_is_clustered(mddev))
		md_cluster_ops->resync_start(mddev, j, max_sectors);

	blk_start_plug(&plug);
	while (j < max_sectors) {
		sector_t sectors;

		skipped = 0;

		if (!test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
		    ((mddev->curr_resync > mddev->curr_resync_completed &&
		      (mddev->curr_resync - mddev->curr_resync_completed)
		      > (max_sectors >> 4)) ||
		     time_after_eq(jiffies, update_time + UPDATE_FREQUENCY) ||
		     (j - mddev->curr_resync_completed)*2
		     >= mddev->resync_max - mddev->curr_resync_completed
			    )) {
			 
			wait_event(mddev->recovery_wait,
				   atomic_read(&mddev->recovery_active) == 0);
			mddev->curr_resync_completed = j;
			if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery) &&
			    j > mddev->recovery_cp)
				mddev->recovery_cp = j;
			update_time = jiffies;
			set_bit(MD_CHANGE_CLEAN, &mddev->flags);
			sysfs_notify(&mddev->kobj, NULL, "sync_completed");
		}

		while (j >= mddev->resync_max &&
		       !test_bit(MD_RECOVERY_INTR, &mddev->recovery)) {
			 
			flush_signals(current);  
			wait_event_interruptible(mddev->recovery_wait,
						 mddev->resync_max > j
						 || test_bit(MD_RECOVERY_INTR,
							     &mddev->recovery));
		}

		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			break;

		sectors = mddev->pers->sync_request(mddev, j, &skipped);
		if (sectors == 0) {
			set_bit(MD_RECOVERY_INTR, &mddev->recovery);
			break;
		}

		if (!skipped) {  
			io_sectors += sectors;
			atomic_add(sectors, &mddev->recovery_active);
		}

		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			break;

		j += sectors;
		if (j > 2)
			mddev->curr_resync = j;
		if (mddev_is_clustered(mddev))
			md_cluster_ops->resync_info_update(mddev, j, max_sectors);
		mddev->curr_mark_cnt = io_sectors;
		if (last_check == 0)
			 
			md_new_event(mddev);

		if (last_check + window > io_sectors || j == max_sectors)
			continue;

		last_check = io_sectors;
	repeat:
		if (time_after_eq(jiffies, mark[last_mark] + SYNC_MARK_STEP )) {
			 
			int next = (last_mark+1) % SYNC_MARKS;

			mddev->resync_mark = mark[next];
			mddev->resync_mark_cnt = mark_cnt[next];
			mark[next] = jiffies;
			mark_cnt[next] = io_sectors - atomic_read(&mddev->recovery_active);
			last_mark = next;
		}

		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			break;

		 
		cond_resched();

		recovery_done = io_sectors - atomic_read(&mddev->recovery_active);
		currspeed = ((unsigned long)(recovery_done - mddev->resync_mark_cnt))/2
			/((jiffies-mddev->resync_mark)/HZ +1) +1;

		if (currspeed > speed_min(mddev)) {
			if (currspeed > speed_max(mddev)) {
				msleep(500);
				goto repeat;
			}
			if (!is_mddev_idle(mddev, 0)) {
				 
				wait_event(mddev->recovery_wait,
					   !atomic_read(&mddev->recovery_active));
			}
		}
	}
	printk(KERN_INFO "md: %s: %s %s.\n",mdname(mddev), desc,
	       test_bit(MD_RECOVERY_INTR, &mddev->recovery)
	       ? "interrupted" : "done");
	 
	blk_finish_plug(&plug);
	wait_event(mddev->recovery_wait, !atomic_read(&mddev->recovery_active));

	 
	mddev->pers->sync_request(mddev, max_sectors, &skipped);

	if (mddev_is_clustered(mddev))
		md_cluster_ops->resync_finish(mddev);

	if (!test_bit(MD_RECOVERY_CHECK, &mddev->recovery) &&
	    mddev->curr_resync > 2) {
		if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
			if (test_bit(MD_RECOVERY_INTR, &mddev->recovery)) {
				if (mddev->curr_resync >= mddev->recovery_cp) {
					printk(KERN_INFO
					       "md: checkpointing %s of %s.\n",
					       desc, mdname(mddev));
					if (test_bit(MD_RECOVERY_ERROR,
						&mddev->recovery))
						mddev->recovery_cp =
							mddev->curr_resync_completed;
					else
						mddev->recovery_cp =
							mddev->curr_resync;
				}
			} else
				mddev->recovery_cp = MaxSector;
		} else {
			if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery))
				mddev->curr_resync = MaxSector;
			rcu_read_lock();
			rdev_for_each_rcu(rdev, mddev)
				if (rdev->raid_disk >= 0 &&
				    mddev->delta_disks >= 0 &&
				    !test_bit(Faulty, &rdev->flags) &&
				    !test_bit(In_sync, &rdev->flags) &&
				    rdev->recovery_offset < mddev->curr_resync)
					rdev->recovery_offset = mddev->curr_resync;
			rcu_read_unlock();
		}
	}
 skip:
	set_bit(MD_CHANGE_DEVS, &mddev->flags);

	spin_lock(&mddev->lock);
	if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery)) {
		 
		if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
			mddev->resync_min = 0;
		mddev->resync_max = MaxSector;
	} else if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
		mddev->resync_min = mddev->curr_resync_completed;
	mddev->curr_resync = 0;
	spin_unlock(&mddev->lock);

	wake_up(&resync_wait);
	set_bit(MD_RECOVERY_DONE, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
	return;
}
