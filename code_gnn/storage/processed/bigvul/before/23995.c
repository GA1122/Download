static int airo_set_nick(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *dwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;

	 
	if(dwrq->length > 16) {
		return -E2BIG;
	}
	readConfigRid(local, 1);
	memset(local->config.nodeName, 0, sizeof(local->config.nodeName));
	memcpy(local->config.nodeName, extra, dwrq->length);
	set_bit (FLAG_COMMIT, &local->flags);

	return -EINPROGRESS;		 
}