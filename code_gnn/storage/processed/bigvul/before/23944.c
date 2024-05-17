static int airo_config_commit(struct net_device *dev,
			      struct iw_request_info *info,	 
			      void *zwrq,			 
			      char *extra)			 
{
	struct airo_info *local = dev->ml_priv;

	if (!test_bit (FLAG_COMMIT, &local->flags))
		return 0;

	 
	disable_MAC(local, 1);
	if (test_bit (FLAG_RESET, &local->flags)) {
		APListRid APList_rid;
		SsidRid SSID_rid;

		readAPListRid(local, &APList_rid);
		readSsidRid(local, &SSID_rid);
		if (test_bit(FLAG_MPI,&local->flags))
			setup_card(local, dev->dev_addr, 1 );
		else
			reset_airo_card(dev);
		disable_MAC(local, 1);
		writeSsidRid(local, &SSID_rid, 1);
		writeAPListRid(local, &APList_rid, 1);
	}
	if (down_interruptible(&local->sem))
		return -ERESTARTSYS;
	writeConfigRid(local, 0);
	enable_MAC(local, 0);
	if (test_bit (FLAG_RESET, &local->flags))
		airo_set_promisc(local);
	else
		up(&local->sem);

	return 0;
}