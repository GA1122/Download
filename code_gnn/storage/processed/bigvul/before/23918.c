static void fr_start(struct net_device *dev)
{
	hdlc_device *hdlc = dev_to_hdlc(dev);
#ifdef DEBUG_LINK
	printk(KERN_DEBUG "fr_start\n");
#endif
	if (state(hdlc)->settings.lmi != LMI_NONE) {
		state(hdlc)->reliable = 0;
		state(hdlc)->dce_changed = 1;
		state(hdlc)->request = 0;
		state(hdlc)->fullrep_sent = 0;
		state(hdlc)->last_errors = 0xFFFFFFFF;
		state(hdlc)->n391cnt = 0;
		state(hdlc)->txseq = state(hdlc)->rxseq = 0;

		init_timer(&state(hdlc)->timer);
		 
		state(hdlc)->timer.expires = jiffies + HZ;
		state(hdlc)->timer.function = fr_timer;
		state(hdlc)->timer.data = (unsigned long)dev;
		add_timer(&state(hdlc)->timer);
	} else
		fr_set_link_state(1, dev);
}