 static int dcbnl_getperm_hwaddr(struct net_device *netdev, struct nlmsghdr *nlh,
 				u32 seq, struct nlattr **tb, struct sk_buff *skb)
 {
 	u8 perm_addr[MAX_ADDR_LEN];
 
  	if (!netdev->dcbnl_ops->getpermhwaddr)
  		return -EOPNOTSUPP;
  
// 	memset(perm_addr, 0, sizeof(perm_addr));
  	netdev->dcbnl_ops->getpermhwaddr(netdev, perm_addr);
  
  	return nla_put(skb, DCB_ATTR_PERM_HWADDR, sizeof(perm_addr), perm_addr);
 }