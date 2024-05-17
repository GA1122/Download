static struct net_device_stats *airo_get_stats(struct net_device *dev)
{
	struct airo_info *local =  dev->ml_priv;

	if (!test_bit(JOB_STATS, &local->jobs)) {
		 
		if (down_trylock(&local->sem) != 0) {
			set_bit(JOB_STATS, &local->jobs);
			wake_up_interruptible(&local->thr_wait);
		} else
			airo_read_stats(dev);
	}

	return &dev->stats;
}