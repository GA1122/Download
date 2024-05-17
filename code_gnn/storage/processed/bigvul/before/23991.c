static int airo_set_frag(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *vwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;
	int fthr = vwrq->value;

	if(vwrq->disabled)
		fthr = AIRO_DEF_MTU;
	if((fthr < 256) || (fthr > AIRO_DEF_MTU)) {
		return -EINVAL;
	}
	fthr &= ~0x1;	 
	readConfigRid(local, 1);
	local->config.fragThresh = cpu_to_le16(fthr);
	set_bit (FLAG_COMMIT, &local->flags);

	return -EINPROGRESS;		 
}