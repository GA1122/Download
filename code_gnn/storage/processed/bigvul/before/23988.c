static int airo_set_encode(struct net_device *dev,
			   struct iw_request_info *info,
			   struct iw_point *dwrq,
			   char *extra)
{
	struct airo_info *local = dev->ml_priv;
	int perm = (dwrq->flags & IW_ENCODE_TEMP ? 0 : 1);
	__le16 currentAuthType = local->config.authType;
	int rc = 0;

	if (!local->wep_capable)
		return -EOPNOTSUPP;

	readConfigRid(local, 1);

	 
	if (dwrq->length > 0) {
		wep_key_t key;
		int index = (dwrq->flags & IW_ENCODE_INDEX) - 1;
		int current_index;

		 
		if (dwrq->length > MAX_KEY_SIZE) {
			return -EINVAL;
		}

		current_index = get_wep_tx_idx(local);
		if (current_index < 0)
			current_index = 0;

		 
		if (!valid_index(local, index))
			index = current_index;

		 
		if (dwrq->length > MIN_KEY_SIZE)
			key.len = MAX_KEY_SIZE;
		else
			key.len = MIN_KEY_SIZE;
		 
		if(!(dwrq->flags & IW_ENCODE_NOKEY)) {
			 
			memset(key.key, 0, MAX_KEY_SIZE);
			 
			memcpy(key.key, extra, dwrq->length);
			 
			rc = set_wep_key(local, index, key.key, key.len, perm, 1);
			if (rc < 0) {
				airo_print_err(local->dev->name, "failed to set"
				               " WEP key at index %d: %d.",
				               index, rc);
				return rc;
			}
		}
		 
		if((index == current_index) && (key.len > 0) &&
		   (local->config.authType == AUTH_OPEN)) {
			local->config.authType = AUTH_ENCRYPT;
		}
	} else {
		 
		int index = (dwrq->flags & IW_ENCODE_INDEX) - 1;
		if (valid_index(local, index)) {
			rc = set_wep_tx_idx(local, index, perm, 1);
			if (rc < 0) {
				airo_print_err(local->dev->name, "failed to set"
				               " WEP transmit index to %d: %d.",
				               index, rc);
				return rc;
			}
		} else {
			 
			if (!(dwrq->flags & IW_ENCODE_MODE))
				return -EINVAL;
		}
	}
	 
	if(dwrq->flags & IW_ENCODE_DISABLED)
		local->config.authType = AUTH_OPEN;	 
	if(dwrq->flags & IW_ENCODE_RESTRICTED)
		local->config.authType = AUTH_SHAREDKEY;	 
	if(dwrq->flags & IW_ENCODE_OPEN)
		local->config.authType = AUTH_ENCRYPT;	 
	 
	if (local->config.authType != currentAuthType)
		set_bit (FLAG_COMMIT, &local->flags);
	return -EINPROGRESS;		 
}
