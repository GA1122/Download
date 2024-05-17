static int airo_set_mode(struct net_device *dev,
			 struct iw_request_info *info,
			 __u32 *uwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;
	int reset = 0;

	readConfigRid(local, 1);
	if (sniffing_mode(local))
		reset = 1;

	switch(*uwrq) {
		case IW_MODE_ADHOC:
			local->config.opmode &= ~MODE_CFG_MASK;
			local->config.opmode |= MODE_STA_IBSS;
			local->config.rmode &= ~RXMODE_FULL_MASK;
			local->config.scanMode = SCANMODE_ACTIVE;
			clear_bit (FLAG_802_11, &local->flags);
			break;
		case IW_MODE_INFRA:
			local->config.opmode &= ~MODE_CFG_MASK;
			local->config.opmode |= MODE_STA_ESS;
			local->config.rmode &= ~RXMODE_FULL_MASK;
			local->config.scanMode = SCANMODE_ACTIVE;
			clear_bit (FLAG_802_11, &local->flags);
			break;
		case IW_MODE_MASTER:
			local->config.opmode &= ~MODE_CFG_MASK;
			local->config.opmode |= MODE_AP;
			local->config.rmode &= ~RXMODE_FULL_MASK;
			local->config.scanMode = SCANMODE_ACTIVE;
			clear_bit (FLAG_802_11, &local->flags);
			break;
		case IW_MODE_REPEAT:
			local->config.opmode &= ~MODE_CFG_MASK;
			local->config.opmode |= MODE_AP_RPTR;
			local->config.rmode &= ~RXMODE_FULL_MASK;
			local->config.scanMode = SCANMODE_ACTIVE;
			clear_bit (FLAG_802_11, &local->flags);
			break;
		case IW_MODE_MONITOR:
			local->config.opmode &= ~MODE_CFG_MASK;
			local->config.opmode |= MODE_STA_ESS;
			local->config.rmode &= ~RXMODE_FULL_MASK;
			local->config.rmode |= RXMODE_RFMON | RXMODE_DISABLE_802_3_HEADER;
			local->config.scanMode = SCANMODE_PASSIVE;
			set_bit (FLAG_802_11, &local->flags);
			break;
		default:
			return -EINVAL;
	}
	if (reset)
		set_bit (FLAG_RESET, &local->flags);
	set_bit (FLAG_COMMIT, &local->flags);

	return -EINPROGRESS;		 
}