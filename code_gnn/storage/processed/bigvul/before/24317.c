static int ieee80211_change_mtu(struct net_device *dev, int new_mtu)
{
	int meshhdrlen;
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);

	meshhdrlen = (sdata->vif.type == NL80211_IFTYPE_MESH_POINT) ? 5 : 0;

	 
	if (new_mtu < 256 ||
	    new_mtu > IEEE80211_MAX_DATA_LEN - 24 - 6 - meshhdrlen) {
		return -EINVAL;
	}

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	printk(KERN_DEBUG "%s: setting MTU %d\n", dev->name, new_mtu);
#endif  
	dev->mtu = new_mtu;
	return 0;
}