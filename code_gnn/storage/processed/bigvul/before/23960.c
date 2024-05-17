static int airo_get_rate(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_param *vwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;
	StatusRid status_rid;		 

	readStatusRid(local, &status_rid, 1);

	vwrq->value = le16_to_cpu(status_rid.currentXmitRate) * 500000;
	 
	readConfigRid(local, 1);
	vwrq->fixed = (local->config.rates[1] == 0);

	return 0;
}