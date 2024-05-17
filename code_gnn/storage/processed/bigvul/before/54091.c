static size_t inet_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct ifaddrmsg))
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(IFNAMSIZ)  
	       + nla_total_size(4)   
	       + nla_total_size(sizeof(struct ifa_cacheinfo));  
}
