static int writerids(struct net_device *dev, aironet_ioctl *comp) {
	struct airo_info *ai = dev->ml_priv;
	int  ridcode;
        int  enabled;
	static int (* writer)(struct airo_info *, u16 rid, const void *, int, int);
	unsigned char *iobuf;

	 
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (test_bit(FLAG_FLASHING, &ai->flags))
		return -EIO;

	ridcode = 0;
	writer = do_writerid;

	switch(comp->command)
	{
	case AIROPSIDS:     ridcode = RID_SSID;         break;
	case AIROPCAP:      ridcode = RID_CAPABILITIES; break;
	case AIROPAPLIST:   ridcode = RID_APLIST;       break;
	case AIROPCFG: ai->config.len = 0;
			    clear_bit(FLAG_COMMIT, &ai->flags);
			    ridcode = RID_CONFIG;       break;
	case AIROPWEPKEYNV: ridcode = RID_WEP_PERM;     break;
	case AIROPLEAPUSR:  ridcode = RID_LEAPUSERNAME; break;
	case AIROPLEAPPWD:  ridcode = RID_LEAPPASSWORD; break;
	case AIROPWEPKEY:   ridcode = RID_WEP_TEMP; writer = PC4500_writerid;
		break;
	case AIROPLEAPUSR+1: ridcode = 0xFF2A;          break;
	case AIROPLEAPUSR+2: ridcode = 0xFF2B;          break;

		 
	case AIROPMACON:
		if (enable_MAC(ai, 1) != 0)
			return -EIO;
		return 0;

		 
	case AIROPMACOFF:
		disable_MAC(ai, 1);
		return 0;

		 
	case AIROPSTCLR:
		if ((iobuf = kmalloc(RIDSIZE, GFP_KERNEL)) == NULL)
			return -ENOMEM;

		PC4500_readrid(ai,RID_STATSDELTACLEAR,iobuf,RIDSIZE, 1);

		enabled = ai->micstats.enabled;
		memset(&ai->micstats,0,sizeof(ai->micstats));
		ai->micstats.enabled = enabled;

		if (copy_to_user(comp->data, iobuf,
				 min((int)comp->len, (int)RIDSIZE))) {
			kfree (iobuf);
			return -EFAULT;
		}
		kfree (iobuf);
		return 0;

	default:
		return -EOPNOTSUPP;	 
	}
	if(comp->len > RIDSIZE)
		return -EINVAL;

	if ((iobuf = kmalloc(RIDSIZE, GFP_KERNEL)) == NULL)
		return -ENOMEM;

	if (copy_from_user(iobuf,comp->data,comp->len)) {
		kfree (iobuf);
		return -EFAULT;
	}

	if (comp->command == AIROPCFG) {
		ConfigRid *cfg = (ConfigRid *)iobuf;

		if (test_bit(FLAG_MIC_CAPABLE, &ai->flags))
			cfg->opmode |= MODE_MIC;

		if ((cfg->opmode & MODE_CFG_MASK) == MODE_STA_IBSS)
			set_bit (FLAG_ADHOC, &ai->flags);
		else
			clear_bit (FLAG_ADHOC, &ai->flags);
	}

	if((*writer)(ai, ridcode, iobuf,comp->len,1)) {
		kfree (iobuf);
		return -EIO;
	}
	kfree (iobuf);
	return 0;
}