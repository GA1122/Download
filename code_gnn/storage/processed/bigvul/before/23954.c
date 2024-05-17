static int airo_get_mode(struct net_device *dev,
			 struct iw_request_info *info,
			 __u32 *uwrq,
			 char *extra)
{
	struct airo_info *local = dev->ml_priv;

	readConfigRid(local, 1);
	 
	switch (local->config.opmode & MODE_CFG_MASK) {
		case MODE_STA_ESS:
			*uwrq = IW_MODE_INFRA;
			break;
		case MODE_AP:
			*uwrq = IW_MODE_MASTER;
			break;
		case MODE_AP_RPTR:
			*uwrq = IW_MODE_REPEAT;
			break;
		default:
			*uwrq = IW_MODE_ADHOC;
	}

	return 0;
}