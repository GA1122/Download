static noinline size_t if_nlmsg_size(const struct net_device *dev,
				     u32 ext_filter_mask)
{
	return NLMSG_ALIGN(sizeof(struct ifinfomsg))
	       + nla_total_size(IFNAMSIZ)  
	       + nla_total_size(IFALIASZ)  
	       + nla_total_size(IFNAMSIZ)  
	       + nla_total_size(sizeof(struct rtnl_link_ifmap))
	       + nla_total_size(sizeof(struct rtnl_link_stats))
	       + nla_total_size(sizeof(struct rtnl_link_stats64))
	       + nla_total_size(MAX_ADDR_LEN)  
	       + nla_total_size(MAX_ADDR_LEN)  
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(1)  
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(4)  
	       + nla_total_size(1)  
	       + nla_total_size(1)  
	       + nla_total_size(ext_filter_mask
			        & RTEXT_FILTER_VF ? 4 : 0)  
	       + rtnl_vfinfo_size(dev, ext_filter_mask)  
	       + rtnl_port_size(dev)  
	       + rtnl_link_get_size(dev)  
	       + rtnl_link_get_af_size(dev);  
}