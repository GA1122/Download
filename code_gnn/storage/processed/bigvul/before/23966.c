static int airo_get_txpow(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_param *vwrq,
			  char *extra)
{
	struct airo_info *local = dev->ml_priv;

	readConfigRid(local, 1);
	vwrq->value = le16_to_cpu(local->config.txPower);
	vwrq->fixed = 1;	 
	vwrq->disabled = test_bit(FLAG_RADIO_OFF, &local->flags);
	vwrq->flags = IW_TXPOW_MWATT;

	return 0;
}