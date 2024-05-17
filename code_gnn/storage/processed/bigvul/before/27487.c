static size_t ipgre_get_size(const struct net_device *dev)
{
	return
		 
		nla_total_size(4) +
		 
		nla_total_size(2) +
		 
		nla_total_size(2) +
		 
		nla_total_size(4) +
		 
		nla_total_size(4) +
		 
		nla_total_size(4) +
		 
		nla_total_size(4) +
		 
		nla_total_size(1) +
		 
		nla_total_size(1) +
		 
		nla_total_size(1) +
		0;
}
