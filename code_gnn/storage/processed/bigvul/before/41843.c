static inline size_t inet6_ifla6_size(void)
{
	return nla_total_size(4)  
	     + nla_total_size(sizeof(struct ifla_cacheinfo))
	     + nla_total_size(DEVCONF_MAX * 4)  
	     + nla_total_size(IPSTATS_MIB_MAX * 8)  
	     + nla_total_size(ICMP6_MIB_MAX * 8)  
	     + nla_total_size(sizeof(struct in6_addr));  
}
