static int airo_set_freq(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_freq *fwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;
	int rc = -EINPROGRESS;		 

	 
	if(fwrq->e == 1) {
		int f = fwrq->m / 100000;

		 
		fwrq->e = 0;
		fwrq->m = ieee80211_freq_to_dsss_chan(f);
	}
	 
	if((fwrq->m > 1000) || (fwrq->e > 0))
		rc = -EOPNOTSUPP;
	else {
		int channel = fwrq->m;
		 
		if((channel < 1) || (channel > 14)) {
			airo_print_dbg(dev->name, "New channel value of %d is invalid!",
				fwrq->m);
			rc = -EINVAL;
		} else {
			readConfigRid(local, 1);
			 
			local->config.channelSet = cpu_to_le16(channel);
			set_bit (FLAG_COMMIT, &local->flags);
		}
	}
	return rc;
}