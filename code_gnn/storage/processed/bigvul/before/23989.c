static int airo_set_encodeext(struct net_device *dev,
			   struct iw_request_info *info,
			    union iwreq_data *wrqu,
			    char *extra)
{
	struct airo_info *local = dev->ml_priv;
	struct iw_point *encoding = &wrqu->encoding;
	struct iw_encode_ext *ext = (struct iw_encode_ext *)extra;
	int perm = ( encoding->flags & IW_ENCODE_TEMP ? 0 : 1 );
	__le16 currentAuthType = local->config.authType;
	int idx, key_len, alg = ext->alg, set_key = 1, rc;
	wep_key_t key;

	if (!local->wep_capable)
		return -EOPNOTSUPP;

	readConfigRid(local, 1);

	 
	idx = encoding->flags & IW_ENCODE_INDEX;
	if (idx) {
		if (!valid_index(local, idx - 1))
			return -EINVAL;
		idx--;
	} else {
		idx = get_wep_tx_idx(local);
		if (idx < 0)
			idx = 0;
	}

	if (encoding->flags & IW_ENCODE_DISABLED)
		alg = IW_ENCODE_ALG_NONE;

	if (ext->ext_flags & IW_ENCODE_EXT_SET_TX_KEY) {
		 
		rc = set_wep_tx_idx(local, idx, perm, 1);
		if (rc < 0) {
			airo_print_err(local->dev->name, "failed to set "
			               "WEP transmit index to %d: %d.",
			               idx, rc);
			return rc;
		}
		set_key = ext->key_len > 0 ? 1 : 0;
	}

	if (set_key) {
		 
		memset(key.key, 0, MAX_KEY_SIZE);
		switch (alg) {
		case IW_ENCODE_ALG_NONE:
			key.len = 0;
			break;
		case IW_ENCODE_ALG_WEP:
			if (ext->key_len > MIN_KEY_SIZE) {
				key.len = MAX_KEY_SIZE;
			} else if (ext->key_len > 0) {
				key.len = MIN_KEY_SIZE;
			} else {
				return -EINVAL;
			}
			key_len = min (ext->key_len, key.len);
			memcpy(key.key, ext->key, key_len);
			break;
		default:
			return -EINVAL;
		}
		if (key.len == 0) {
			rc = set_wep_tx_idx(local, idx, perm, 1);
			if (rc < 0) {
				airo_print_err(local->dev->name,
					       "failed to set WEP transmit index to %d: %d.",
					       idx, rc);
				return rc;
			}
		} else {
			rc = set_wep_key(local, idx, key.key, key.len, perm, 1);
			if (rc < 0) {
				airo_print_err(local->dev->name,
					       "failed to set WEP key at index %d: %d.",
					       idx, rc);
				return rc;
			}
		}
	}

	 
	if(encoding->flags & IW_ENCODE_DISABLED)
		local->config.authType = AUTH_OPEN;	 
	if(encoding->flags & IW_ENCODE_RESTRICTED)
		local->config.authType = AUTH_SHAREDKEY;	 
	if(encoding->flags & IW_ENCODE_OPEN)
		local->config.authType = AUTH_ENCRYPT;	 
	 
	if (local->config.authType != currentAuthType)
		set_bit (FLAG_COMMIT, &local->flags);

	return -EINPROGRESS;
}