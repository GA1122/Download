static size_t rtnl_port_size(const struct net_device *dev)
{
	size_t port_size = nla_total_size(4)		 
		+ nla_total_size(PORT_PROFILE_MAX)	 
		+ nla_total_size(sizeof(struct ifla_port_vsi))
							 
		+ nla_total_size(PORT_UUID_MAX)		 
		+ nla_total_size(PORT_UUID_MAX)		 
		+ nla_total_size(1)			 
		+ nla_total_size(2);			 
	size_t vf_ports_size = nla_total_size(sizeof(struct nlattr));
	size_t vf_port_size = nla_total_size(sizeof(struct nlattr))
		+ port_size;
	size_t port_self_size = nla_total_size(sizeof(struct nlattr))
		+ port_size;

	if (!dev->netdev_ops->ndo_get_vf_port || !dev->dev.parent)
		return 0;
	if (dev_num_vf(dev->dev.parent))
		return port_self_size + vf_ports_size +
			vf_port_size * dev_num_vf(dev->dev.parent);
	else
		return port_self_size;
}
