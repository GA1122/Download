static inline int inet6_ifaddr_msgsize(void)
{
	return NLMSG_ALIGN(sizeof(struct ifaddrmsg))
	       + nla_total_size(16)  
	       + nla_total_size(16)  
	       + nla_total_size(sizeof(struct ifa_cacheinfo))
	       + nla_total_size(4)   ;
}
