static int readrids(struct net_device *dev, aironet_ioctl *comp) {
	unsigned short ridcode;
	unsigned char *iobuf;
	int len;
	struct airo_info *ai = dev->ml_priv;

	if (test_bit(FLAG_FLASHING, &ai->flags))
		return -EIO;

	switch(comp->command)
	{
	case AIROGCAP:      ridcode = RID_CAPABILITIES; break;
	case AIROGCFG:      ridcode = RID_CONFIG;
		if (test_bit(FLAG_COMMIT, &ai->flags)) {
			disable_MAC (ai, 1);
			writeConfigRid (ai, 1);
			enable_MAC(ai, 1);
		}
		break;
	case AIROGSLIST:    ridcode = RID_SSID;         break;
	case AIROGVLIST:    ridcode = RID_APLIST;       break;
	case AIROGDRVNAM:   ridcode = RID_DRVNAME;      break;
	case AIROGEHTENC:   ridcode = RID_ETHERENCAP;   break;
	case AIROGWEPKTMP:  ridcode = RID_WEP_TEMP;
		 
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		break;
	case AIROGWEPKNV:   ridcode = RID_WEP_PERM;
		 
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		break;
	case AIROGSTAT:     ridcode = RID_STATUS;       break;
	case AIROGSTATSD32: ridcode = RID_STATSDELTA;   break;
	case AIROGSTATSC32: ridcode = RID_STATS;        break;
	case AIROGMICSTATS:
		if (copy_to_user(comp->data, &ai->micstats,
				 min((int)comp->len,(int)sizeof(ai->micstats))))
			return -EFAULT;
		return 0;
	case AIRORRID:      ridcode = comp->ridnum;     break;
	default:
		return -EINVAL;
		break;
	}

	if ((iobuf = kmalloc(RIDSIZE, GFP_KERNEL)) == NULL)
		return -ENOMEM;

	PC4500_readrid(ai,ridcode,iobuf,RIDSIZE, 1);
	 
	len = comp->len;

	if (copy_to_user(comp->data, iobuf, min(len, (int)RIDSIZE))) {
		kfree (iobuf);
		return -EFAULT;
	}
	kfree (iobuf);
	return 0;
}