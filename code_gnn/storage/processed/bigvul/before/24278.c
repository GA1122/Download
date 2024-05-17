static int vlan_dev_init(struct net_device *dev)
{
	struct net_device *real_dev = vlan_dev_info(dev)->real_dev;
	int subclass = 0;

	netif_carrier_off(dev);

	 
	dev->flags  = real_dev->flags & ~(IFF_UP | IFF_PROMISC | IFF_ALLMULTI |
					  IFF_MASTER | IFF_SLAVE);
	dev->iflink = real_dev->ifindex;
	dev->state  = (real_dev->state & ((1<<__LINK_STATE_NOCARRIER) |
					  (1<<__LINK_STATE_DORMANT))) |
		      (1<<__LINK_STATE_PRESENT);

	dev->hw_features = NETIF_F_ALL_CSUM | NETIF_F_SG |
			   NETIF_F_FRAGLIST | NETIF_F_ALL_TSO |
			   NETIF_F_HIGHDMA | NETIF_F_SCTP_CSUM |
			   NETIF_F_ALL_FCOE;

	dev->features |= real_dev->vlan_features | NETIF_F_LLTX;
	dev->gso_max_size = real_dev->gso_max_size;

	 
	dev->dev_id = real_dev->dev_id;

	if (is_zero_ether_addr(dev->dev_addr))
		memcpy(dev->dev_addr, real_dev->dev_addr, dev->addr_len);
	if (is_zero_ether_addr(dev->broadcast))
		memcpy(dev->broadcast, real_dev->broadcast, dev->addr_len);

#if defined(CONFIG_FCOE) || defined(CONFIG_FCOE_MODULE)
	dev->fcoe_ddp_xid = real_dev->fcoe_ddp_xid;
#endif

	dev->needed_headroom = real_dev->needed_headroom;
	if (real_dev->features & NETIF_F_HW_VLAN_TX) {
		dev->header_ops      = real_dev->header_ops;
		dev->hard_header_len = real_dev->hard_header_len;
	} else {
		dev->header_ops      = &vlan_header_ops;
		dev->hard_header_len = real_dev->hard_header_len + VLAN_HLEN;
	}

	dev->netdev_ops = &vlan_netdev_ops;

	if (is_vlan_dev(real_dev))
		subclass = 1;

	vlan_dev_set_lockdep_class(dev, subclass);

	vlan_dev_info(dev)->vlan_pcpu_stats = alloc_percpu(struct vlan_pcpu_stats);
	if (!vlan_dev_info(dev)->vlan_pcpu_stats)
		return -ENOMEM;

	return 0;
}