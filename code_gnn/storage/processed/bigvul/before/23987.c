static int airo_set_auth(struct net_device *dev,
			       struct iw_request_info *info,
			       union iwreq_data *wrqu, char *extra)
{
	struct airo_info *local = dev->ml_priv;
	struct iw_param *param = &wrqu->param;
	__le16 currentAuthType = local->config.authType;

	switch (param->flags & IW_AUTH_INDEX) {
	case IW_AUTH_WPA_VERSION:
	case IW_AUTH_CIPHER_PAIRWISE:
	case IW_AUTH_CIPHER_GROUP:
	case IW_AUTH_KEY_MGMT:
	case IW_AUTH_RX_UNENCRYPTED_EAPOL:
	case IW_AUTH_PRIVACY_INVOKED:
		 
		break;

	case IW_AUTH_DROP_UNENCRYPTED:
		if (param->value) {
			 
			if (currentAuthType == AUTH_OPEN)
				local->config.authType = AUTH_ENCRYPT;
		} else {
			local->config.authType = AUTH_OPEN;
		}

		 
		if (local->config.authType != currentAuthType)
			set_bit (FLAG_COMMIT, &local->flags);
		break;

	case IW_AUTH_80211_AUTH_ALG: {
			 
			if (param->value & IW_AUTH_ALG_SHARED_KEY) {
				local->config.authType = AUTH_SHAREDKEY;
			} else if (param->value & IW_AUTH_ALG_OPEN_SYSTEM) {
				local->config.authType = AUTH_ENCRYPT;
			} else
				return -EINVAL;

			 
			if (local->config.authType != currentAuthType)
				set_bit (FLAG_COMMIT, &local->flags);
			break;
		}

	case IW_AUTH_WPA_ENABLED:
		 
		if (param->value > 0)
			return -EOPNOTSUPP;
		break;

	default:
		return -EOPNOTSUPP;
	}
	return -EINPROGRESS;
}
