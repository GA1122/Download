static int set_offload(struct tun_struct *tun, unsigned long arg)
{
	netdev_features_t features = 0;

	if (arg & TUN_F_CSUM) {
		features |= NETIF_F_HW_CSUM;
		arg &= ~TUN_F_CSUM;

		if (arg & (TUN_F_TSO4|TUN_F_TSO6)) {
			if (arg & TUN_F_TSO_ECN) {
				features |= NETIF_F_TSO_ECN;
				arg &= ~TUN_F_TSO_ECN;
			}
			if (arg & TUN_F_TSO4)
				features |= NETIF_F_TSO;
			if (arg & TUN_F_TSO6)
				features |= NETIF_F_TSO6;
			arg &= ~(TUN_F_TSO4|TUN_F_TSO6);
		}
	}

	 
	if (arg)
		return -EINVAL;

	tun->set_features = features;
	tun->dev->wanted_features &= ~TUN_USER_FEATURES;
	tun->dev->wanted_features |= features;
	netdev_update_features(tun->dev);

	return 0;
}