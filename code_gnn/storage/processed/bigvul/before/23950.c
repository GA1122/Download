static int airo_get_encode(struct net_device *dev,
			   struct iw_request_info *info,
			   struct iw_point *dwrq,
			   char *extra)
{
	struct airo_info *local = dev->ml_priv;
	int index = (dwrq->flags & IW_ENCODE_INDEX) - 1;
	int wep_key_len;
	u8 buf[16];

	if (!local->wep_capable)
		return -EOPNOTSUPP;

	readConfigRid(local, 1);

	 
	switch(local->config.authType)	{
		case AUTH_ENCRYPT:
			dwrq->flags = IW_ENCODE_OPEN;
			break;
		case AUTH_SHAREDKEY:
			dwrq->flags = IW_ENCODE_RESTRICTED;
			break;
		default:
		case AUTH_OPEN:
			dwrq->flags = IW_ENCODE_DISABLED;
			break;
	}
	 
	dwrq->flags |= IW_ENCODE_NOKEY;
	memset(extra, 0, 16);

	 
	if (!valid_index(local, index)) {
		index = get_wep_tx_idx(local);
		if (index < 0)
			index = 0;
	}
	dwrq->flags |= index + 1;

	 
	wep_key_len = get_wep_key(local, index, &buf[0], sizeof(buf));
	if (wep_key_len < 0) {
		dwrq->length = 0;
	} else {
		dwrq->length = wep_key_len;
		memcpy(extra, buf, dwrq->length);
	}

	return 0;
}