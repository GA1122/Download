static int airo_set_power(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_param *vwrq,
			  char *extra)
{
	struct airo_info *local = dev->ml_priv;

	readConfigRid(local, 1);
	if (vwrq->disabled) {
		if (sniffing_mode(local))
			return -EINVAL;
		local->config.powerSaveMode = POWERSAVE_CAM;
		local->config.rmode &= ~RXMODE_MASK;
		local->config.rmode |= RXMODE_BC_MC_ADDR;
		set_bit (FLAG_COMMIT, &local->flags);
		return -EINPROGRESS;		 
	}
	if ((vwrq->flags & IW_POWER_TYPE) == IW_POWER_TIMEOUT) {
		local->config.fastListenDelay = cpu_to_le16((vwrq->value + 500) / 1024);
		local->config.powerSaveMode = POWERSAVE_PSPCAM;
		set_bit (FLAG_COMMIT, &local->flags);
	} else if ((vwrq->flags & IW_POWER_TYPE) == IW_POWER_PERIOD) {
		local->config.fastListenInterval =
		local->config.listenInterval =
			cpu_to_le16((vwrq->value + 500) / 1024);
		local->config.powerSaveMode = POWERSAVE_PSPCAM;
		set_bit (FLAG_COMMIT, &local->flags);
	}
	switch (vwrq->flags & IW_POWER_MODE) {
		case IW_POWER_UNICAST_R:
			if (sniffing_mode(local))
				return -EINVAL;
			local->config.rmode &= ~RXMODE_MASK;
			local->config.rmode |= RXMODE_ADDR;
			set_bit (FLAG_COMMIT, &local->flags);
			break;
		case IW_POWER_ALL_R:
			if (sniffing_mode(local))
				return -EINVAL;
			local->config.rmode &= ~RXMODE_MASK;
			local->config.rmode |= RXMODE_BC_MC_ADDR;
			set_bit (FLAG_COMMIT, &local->flags);
		case IW_POWER_ON:
			 
			break;
		default:
			return -EINVAL;
	}
	return -EINPROGRESS;		 
}