static int airo_set_txpow(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_param *vwrq,
			  char *extra)
{
	struct airo_info *local = dev->ml_priv;
	CapabilityRid cap_rid;		 
	int i;
	int rc = -EINVAL;
	__le16 v = cpu_to_le16(vwrq->value);

	readCapabilityRid(local, &cap_rid, 1);

	if (vwrq->disabled) {
		set_bit (FLAG_RADIO_OFF, &local->flags);
		set_bit (FLAG_COMMIT, &local->flags);
		return -EINPROGRESS;		 
	}
	if (vwrq->flags != IW_TXPOW_MWATT) {
		return -EINVAL;
	}
	clear_bit (FLAG_RADIO_OFF, &local->flags);
	for (i = 0; i < 8 && cap_rid.txPowerLevels[i]; i++)
		if (v == cap_rid.txPowerLevels[i]) {
			readConfigRid(local, 1);
			local->config.txPower = v;
			set_bit (FLAG_COMMIT, &local->flags);
			rc = -EINPROGRESS;	 
			break;
		}
	return rc;
}