static size_t inet_get_link_af_size(const struct net_device *dev,
				    u32 ext_filter_mask)
{
	struct in_device *in_dev = rcu_dereference_rtnl(dev->ip_ptr);

	if (!in_dev)
		return 0;

	return nla_total_size(IPV4_DEVCONF_MAX * 4);  
}