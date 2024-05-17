int hostap_set_string(struct net_device *dev, int rid, const char *val)
{
	struct hostap_interface *iface;
	char buf[MAX_SSID_LEN + 2];
	int len;

	iface = netdev_priv(dev);
	len = strlen(val);
	if (len > MAX_SSID_LEN)
		return -1;
	memset(buf, 0, sizeof(buf));
	buf[0] = len;  
	memcpy(buf + 2, val, len);

	return iface->local->func->set_rid(dev, rid, &buf, MAX_SSID_LEN + 2);
}