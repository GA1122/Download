 static void _isdn_setup(struct net_device *dev)
 {
 	isdn_net_local *lp = netdev_priv(dev);
 
 	ether_setup(dev);
  
  	 
  	dev->flags = IFF_NOARP|IFF_POINTOPOINT;
// 
// 	 
// 	dev->priv_flags &= ~IFF_TX_SKB_SHARING;
  	dev->header_ops = NULL;
  	dev->netdev_ops = &isdn_netdev_ops;
  
 	 
 	dev->tx_queue_len = 30;
 
 	lp->p_encap = ISDN_NET_ENCAP_RAWIP;
 	lp->magic = ISDN_NET_MAGIC;
 	lp->last = lp;
 	lp->next = lp;
 	lp->isdn_device = -1;
 	lp->isdn_channel = -1;
 	lp->pre_device = -1;
 	lp->pre_channel = -1;
 	lp->exclusive = -1;
 	lp->ppp_slot = -1;
 	lp->pppbind = -1;
 	skb_queue_head_init(&lp->super_tx_queue);
 	lp->l2_proto = ISDN_PROTO_L2_X75I;
 	lp->l3_proto = ISDN_PROTO_L3_TRANS;
 	lp->triggercps = 6000;
 	lp->slavedelay = 10 * HZ;
 	lp->hupflags = ISDN_INHUP;	 
 	lp->onhtime = 10;	 
 	lp->dialmax = 1;
 	 
 	lp->flags = ISDN_NET_CBHUP | ISDN_NET_DM_MANUAL;
 	lp->cbdelay = 25;	 
 	lp->dialtimeout = -1;   
 	lp->dialwait = 5 * HZ;  
 	lp->dialstarted = 0;    
 	lp->dialwait_timer = 0;   
 }