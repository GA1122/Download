static int airo_set_rate(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *vwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;
	CapabilityRid cap_rid;		 
	u8	brate = 0;
	int	i;

	 
	readCapabilityRid(local, &cap_rid, 1);

	 
	if((vwrq->value < 8) && (vwrq->value >= 0)) {
		 
		 
		brate = cap_rid.supportedRates[vwrq->value];
	} else {
		 
		u8	normvalue = (u8) (vwrq->value/500000);

		 
		for(i = 0 ; i < 8 ; i++) {
			if(normvalue == cap_rid.supportedRates[i]) {
				brate = normvalue;
				break;
			}
		}
	}
	 
	if(vwrq->value == -1) {
		 
		for(i = 0 ; i < 8 ; i++) {
			if(cap_rid.supportedRates[i] == 0)
				break;
		}
		if(i != 0)
			brate = cap_rid.supportedRates[i - 1];
	}
	 
	if(brate == 0) {
		return -EINVAL;
	}

	readConfigRid(local, 1);
	 
	if(vwrq->fixed == 0) {
		 
		memset(local->config.rates, 0, 8);
		for(i = 0 ; i < 8 ; i++) {
			local->config.rates[i] = cap_rid.supportedRates[i];
			if(local->config.rates[i] == brate)
				break;
		}
	} else {
		 
		 
		memset(local->config.rates, 0, 8);
		local->config.rates[0] = brate;
	}
	set_bit (FLAG_COMMIT, &local->flags);

	return -EINPROGRESS;		 
}
