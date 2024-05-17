 static void tun_net_init(struct net_device *dev)
 {
 	struct tun_struct *tun = netdev_priv(dev);
 
 	switch (tun->flags & TUN_TYPE_MASK) {
 	case TUN_TUN_DEV:
 		dev->netdev_ops = &tun_netdev_ops;
 
 		 
 		dev->hard_header_len = 0;
 		dev->addr_len = 0;
 		dev->mtu = 1500;
 
 		 
 		dev->type = ARPHRD_NONE;
 		dev->flags = IFF_POINTOPOINT | IFF_NOARP | IFF_MULTICAST;
 		dev->tx_queue_len = TUN_READQ_SIZE;   
 		break;
 
 	case TUN_TAP_DEV:
  		dev->netdev_ops = &tap_netdev_ops;
  		 
  		ether_setup(dev);
// 		dev->priv_flags &= ~IFF_TX_SKB_SHARING;
  
  		random_ether_addr(dev->dev_addr);
  
 		dev->tx_queue_len = TUN_READQ_SIZE;   
 		break;
 	}
 }