static int airo_set_rts(struct net_device *dev,
			struct iw_request_info *info,
			struct iw_param *vwrq,
			char *extra)
{
	struct airo_info *local = dev->ml_priv;
	int rthr = vwrq->value;

	if(vwrq->disabled)
		rthr = AIRO_DEF_MTU;
	if((rthr < 0) || (rthr > AIRO_DEF_MTU)) {
		return -EINVAL;
	}
	readConfigRid(local, 1);
	local->config.rtsThres = cpu_to_le16(rthr);
	set_bit (FLAG_COMMIT, &local->flags);

	return -EINPROGRESS;		 
}