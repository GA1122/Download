static int airo_set_retry(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_param *vwrq,
			  char *extra)
{
	struct airo_info *local = dev->ml_priv;
	int rc = -EINVAL;

	if(vwrq->disabled) {
		return -EINVAL;
	}
	readConfigRid(local, 1);
	if(vwrq->flags & IW_RETRY_LIMIT) {
		__le16 v = cpu_to_le16(vwrq->value);
		if(vwrq->flags & IW_RETRY_LONG)
			local->config.longRetryLimit = v;
		else if (vwrq->flags & IW_RETRY_SHORT)
			local->config.shortRetryLimit = v;
		else {
			 
			local->config.longRetryLimit = v;
			local->config.shortRetryLimit = v;
		}
		set_bit (FLAG_COMMIT, &local->flags);
		rc = -EINPROGRESS;		 
	}
	if(vwrq->flags & IW_RETRY_LIFETIME) {
		local->config.txLifetime = cpu_to_le16(vwrq->value / 1024);
		set_bit (FLAG_COMMIT, &local->flags);
		rc = -EINPROGRESS;		 
	}
	return rc;
}