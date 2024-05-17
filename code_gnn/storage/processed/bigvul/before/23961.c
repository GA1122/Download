static int airo_get_retry(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_param *vwrq,
			  char *extra)
{
	struct airo_info *local = dev->ml_priv;

	vwrq->disabled = 0;       

	readConfigRid(local, 1);
	 
	if((vwrq->flags & IW_RETRY_TYPE) == IW_RETRY_LIFETIME) {
		vwrq->flags = IW_RETRY_LIFETIME;
		vwrq->value = le16_to_cpu(local->config.txLifetime) * 1024;
	} else if((vwrq->flags & IW_RETRY_LONG)) {
		vwrq->flags = IW_RETRY_LIMIT | IW_RETRY_LONG;
		vwrq->value = le16_to_cpu(local->config.longRetryLimit);
	} else {
		vwrq->flags = IW_RETRY_LIMIT;
		vwrq->value = le16_to_cpu(local->config.shortRetryLimit);
		if(local->config.shortRetryLimit != local->config.longRetryLimit)
			vwrq->flags |= IW_RETRY_SHORT;
	}

	return 0;
}