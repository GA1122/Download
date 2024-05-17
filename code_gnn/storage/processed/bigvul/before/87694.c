static size_t ipip6_get_size(const struct net_device *dev)
{
	return
		 
		nla_total_size(4) +
		 
		nla_total_size(4) +
		 
		nla_total_size(4) +
		 
		nla_total_size(1) +
		 
		nla_total_size(1) +
		 
		nla_total_size(1) +
		 
		nla_total_size(2) +
		 
		nla_total_size(1) +
#ifdef CONFIG_IPV6_SIT_6RD
		 
		nla_total_size(sizeof(struct in6_addr)) +
		 
		nla_total_size(4) +
		 
		nla_total_size(2) +
		 
		nla_total_size(2) +
#endif
		 
		nla_total_size(2) +
		 
		nla_total_size(2) +
		 
		nla_total_size(2) +
		 
		nla_total_size(2) +
		 
		nla_total_size(4) +
		0;
}
