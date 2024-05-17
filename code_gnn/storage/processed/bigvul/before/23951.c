static int airo_get_encodeext(struct net_device *dev,
			    struct iw_request_info *info,
			    union iwreq_data *wrqu,
			    char *extra)
{
	struct airo_info *local = dev->ml_priv;
	struct iw_point *encoding = &wrqu->encoding;
	struct iw_encode_ext *ext = (struct iw_encode_ext *)extra;
	int idx, max_key_len, wep_key_len;
	u8 buf[16];

	if (!local->wep_capable)
		return -EOPNOTSUPP;

	readConfigRid(local, 1);

	max_key_len = encoding->length - sizeof(*ext);
	if (max_key_len < 0)
		return -EINVAL;

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

	encoding->flags = idx + 1;
	memset(ext, 0, sizeof(*ext));

	 
	switch(local->config.authType) {
		case AUTH_ENCRYPT:
			encoding->flags = IW_ENCODE_ALG_WEP | IW_ENCODE_ENABLED;
			break;
		case AUTH_SHAREDKEY:
			encoding->flags = IW_ENCODE_ALG_WEP | IW_ENCODE_ENABLED;
			break;
		default:
		case AUTH_OPEN:
			encoding->flags = IW_ENCODE_ALG_NONE | IW_ENCODE_DISABLED;
			break;
	}
	 
	encoding->flags |= IW_ENCODE_NOKEY;
	memset(extra, 0, 16);
	
	 
	wep_key_len = get_wep_key(local, idx, &buf[0], sizeof(buf));
	if (wep_key_len < 0) {
		ext->key_len = 0;
	} else {
		ext->key_len = wep_key_len;
		memcpy(extra, buf, ext->key_len);
	}

	return 0;
}