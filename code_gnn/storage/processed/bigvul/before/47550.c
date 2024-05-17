int hash_setconfiguration(struct hash_device_data *device_data,
			  struct hash_config *config)
{
	int ret = 0;

	if (config->algorithm != HASH_ALGO_SHA1 &&
	    config->algorithm != HASH_ALGO_SHA256)
		return -EPERM;

	 
	HASH_SET_DATA_FORMAT(config->data_format);

	 
	switch (config->algorithm) {
	case HASH_ALGO_SHA1:
		HASH_SET_BITS(&device_data->base->cr, HASH_CR_ALGO_MASK);
		break;

	case HASH_ALGO_SHA256:
		HASH_CLEAR_BITS(&device_data->base->cr, HASH_CR_ALGO_MASK);
		break;

	default:
		dev_err(device_data->dev, "%s: Incorrect algorithm\n",
			__func__);
		return -EPERM;
	}

	 
	if (HASH_OPER_MODE_HASH == config->oper_mode)
		HASH_CLEAR_BITS(&device_data->base->cr,
				HASH_CR_MODE_MASK);
	else if (HASH_OPER_MODE_HMAC == config->oper_mode) {
		HASH_SET_BITS(&device_data->base->cr, HASH_CR_MODE_MASK);
		if (device_data->current_ctx->keylen > HASH_BLOCK_SIZE) {
			 
			dev_dbg(device_data->dev, "%s: LKEY set\n", __func__);
			HASH_SET_BITS(&device_data->base->cr,
				      HASH_CR_LKEY_MASK);
		} else {
			dev_dbg(device_data->dev, "%s: LKEY cleared\n",
				__func__);
			HASH_CLEAR_BITS(&device_data->base->cr,
					HASH_CR_LKEY_MASK);
		}
	} else {	 
		ret = -EPERM;
		dev_err(device_data->dev, "%s: HASH_INVALID_PARAMETER!\n",
			__func__);
	}
	return ret;
}
