unsigned long netdev_fix_features(unsigned long features, const char *name)
{
	 
	if ((features & NETIF_F_SG) &&
	    !(features & NETIF_F_ALL_CSUM)) {
		if (name)
			printk(KERN_NOTICE "%s: Dropping NETIF_F_SG since no "
			       "checksum feature.\n", name);
		features &= ~NETIF_F_SG;
	}

	 
	if ((features & NETIF_F_TSO) && !(features & NETIF_F_SG)) {
		if (name)
			printk(KERN_NOTICE "%s: Dropping NETIF_F_TSO since no "
			       "SG feature.\n", name);
		features &= ~NETIF_F_TSO;
	}

	if (features & NETIF_F_UFO) {
		if (!(features & NETIF_F_GEN_CSUM)) {
			if (name)
				printk(KERN_ERR "%s: Dropping NETIF_F_UFO "
				       "since no NETIF_F_HW_CSUM feature.\n",
				       name);
			features &= ~NETIF_F_UFO;
		}

		if (!(features & NETIF_F_SG)) {
			if (name)
				printk(KERN_ERR "%s: Dropping NETIF_F_UFO "
				       "since no NETIF_F_SG feature.\n", name);
			features &= ~NETIF_F_UFO;
		}
	}

	return features;
}