static int ieee80211_open(struct net_device *dev)
{
	struct ieee80211_sub_if_data *sdata = IEEE80211_DEV_TO_SUB_IF(dev);
	int err;

	 
	if (!is_valid_ether_addr(dev->dev_addr))
		return -EADDRNOTAVAIL;

	err = ieee80211_check_concurrent_iface(sdata, sdata->vif.type);
	if (err)
		return err;

	return ieee80211_do_open(dev, true);
}