static struct iw_statistics *airo_get_wireless_stats(struct net_device *dev)
{
	struct airo_info *local =  dev->ml_priv;

	if (!test_bit(JOB_WSTATS, &local->jobs)) {
		 
		if (down_trylock(&local->sem) != 0) {
			set_bit(JOB_WSTATS, &local->jobs);
			wake_up_interruptible(&local->thr_wait);
		} else
			airo_read_wireless_stats(local);
	}

	return &local->wstats;
}