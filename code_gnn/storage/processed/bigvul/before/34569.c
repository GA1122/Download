static int macvtap_newlink(struct net *src_net,
			   struct net_device *dev,
			   struct nlattr *tb[],
			   struct nlattr *data[])
{
	 
	return macvlan_common_newlink(src_net, dev, tb, data,
				      macvtap_receive, macvtap_forward);
}