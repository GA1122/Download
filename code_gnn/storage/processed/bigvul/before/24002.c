static int airo_set_sens(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *vwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;

	readConfigRid(local, 1);
	local->config.rssiThreshold =
		cpu_to_le16(vwrq->disabled ? RSSI_DEFAULT : vwrq->value);
	set_bit (FLAG_COMMIT, &local->flags);

	return -EINPROGRESS;		 
}