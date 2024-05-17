static int airo_close(struct net_device *dev) {
	struct airo_info *ai = dev->ml_priv;

	netif_stop_queue(dev);

	if (ai->wifidev != dev) {
#ifdef POWER_ON_DOWN
		 
		set_bit(FLAG_RADIO_DOWN, &ai->flags);
		disable_MAC(ai, 1);
#endif
		disable_interrupts( ai );

		free_irq(dev->irq, dev);

		set_bit(JOB_DIE, &ai->jobs);
		kthread_stop(ai->airo_thread_task);
	}
	return 0;
}
