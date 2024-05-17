static inline size_t bridge_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct ifinfomsg))
		+ nla_total_size(IFNAMSIZ)	 
		+ nla_total_size(MAX_ADDR_LEN)	 
		+ nla_total_size(sizeof(u32))	 
		+ nla_total_size(sizeof(u32))	 
		+ nla_total_size(sizeof(u32))	 
		+ nla_total_size(sizeof(u32))	 
		+ nla_total_size(sizeof(u8))	 
		+ nla_total_size(sizeof(struct nlattr))	 
		+ nla_total_size(sizeof(u16))	 
		+ nla_total_size(sizeof(u16));	 
}
