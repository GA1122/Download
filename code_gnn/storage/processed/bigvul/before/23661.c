isdn_net_receive(struct net_device *ndev, struct sk_buff *skb)
{
	isdn_net_local *lp = netdev_priv(ndev);
	isdn_net_local *olp = lp;	 
#ifdef CONFIG_ISDN_X25
	struct concap_proto *cprot = lp -> netdev -> cprot;
#endif
	lp->transcount += skb->len;

	lp->stats.rx_packets++;
	lp->stats.rx_bytes += skb->len;
	if (lp->master) {
		 
		ndev = lp->master;
		lp = netdev_priv(ndev);
		lp->stats.rx_packets++;
		lp->stats.rx_bytes += skb->len;
	}
	skb->dev = ndev;
	skb->pkt_type = PACKET_HOST;
	skb_reset_mac_header(skb);
#ifdef ISDN_DEBUG_NET_DUMP
	isdn_dumppkt("R:", skb->data, skb->len, 40);
#endif
	switch (lp->p_encap) {
		case ISDN_NET_ENCAP_ETHER:
			 
			olp->huptimer = 0;
			lp->huptimer = 0;
			skb->protocol = isdn_net_type_trans(skb, ndev);
			break;
		case ISDN_NET_ENCAP_UIHDLC:
			 
			olp->huptimer = 0;
			lp->huptimer = 0;
			skb_pull(skb, 2);
			 
		case ISDN_NET_ENCAP_RAWIP:
			 
			olp->huptimer = 0;
			lp->huptimer = 0;
			skb->protocol = htons(ETH_P_IP);
			break;
		case ISDN_NET_ENCAP_CISCOHDLCK:
			isdn_net_ciscohdlck_receive(lp, skb);
			return;
		case ISDN_NET_ENCAP_CISCOHDLC:
			 
			skb_pull(skb, 2);
			 
		case ISDN_NET_ENCAP_IPTYP:
			 
			olp->huptimer = 0;
			lp->huptimer = 0;
			skb->protocol = *(__be16 *)&(skb->data[0]);
			skb_pull(skb, 2);
			if (*(unsigned short *) skb->data == 0xFFFF)
				skb->protocol = htons(ETH_P_802_3);
			break;
#ifdef CONFIG_ISDN_PPP
		case ISDN_NET_ENCAP_SYNCPPP:
			 
			isdn_ppp_receive(lp->netdev, olp, skb);
			return;
#endif

		default:
#ifdef CONFIG_ISDN_X25
		   
			if(cprot) if(cprot -> pops)
				if( cprot -> pops -> data_ind){
					cprot -> pops -> data_ind(cprot,skb);
					return;
				};
#endif  
			printk(KERN_WARNING "%s: unknown encapsulation, dropping\n",
			       lp->netdev->dev->name);
			kfree_skb(skb);
			return;
	}

	netif_rx(skb);
	return;
}
