static int mr6_msgsize(bool unresolved, int maxvif)
{
	size_t len =
		NLMSG_ALIGN(sizeof(struct rtmsg))
		+ nla_total_size(4)	 
		+ nla_total_size(sizeof(struct in6_addr))	 
		+ nla_total_size(sizeof(struct in6_addr))	 
		;

	if (!unresolved)
		len = len
		      + nla_total_size(4)	 
		      + nla_total_size(0)	 
		      + maxvif * NLA_ALIGN(sizeof(struct rtnexthop))
						 
		      + nla_total_size_64bit(sizeof(struct rta_mfc_stats))
		;

	return len;
}
