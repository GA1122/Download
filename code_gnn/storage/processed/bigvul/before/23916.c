static int fr_rx(struct sk_buff *skb)
{
	struct net_device *frad = skb->dev;
	hdlc_device *hdlc = dev_to_hdlc(frad);
	fr_hdr *fh = (fr_hdr*)skb->data;
	u8 *data = skb->data;
	u16 dlci;
	pvc_device *pvc;
	struct net_device *dev = NULL;

	if (skb->len <= 4 || fh->ea1 || data[2] != FR_UI)
		goto rx_error;

	dlci = q922_to_dlci(skb->data);

	if ((dlci == LMI_CCITT_ANSI_DLCI &&
	     (state(hdlc)->settings.lmi == LMI_ANSI ||
	      state(hdlc)->settings.lmi == LMI_CCITT)) ||
	    (dlci == LMI_CISCO_DLCI &&
	     state(hdlc)->settings.lmi == LMI_CISCO)) {
		if (fr_lmi_recv(frad, skb))
			goto rx_error;
		dev_kfree_skb_any(skb);
		return NET_RX_SUCCESS;
	}

	pvc = find_pvc(hdlc, dlci);
	if (!pvc) {
#ifdef DEBUG_PKT
		netdev_info(frad, "No PVC for received frame's DLCI %d\n",
			    dlci);
#endif
		dev_kfree_skb_any(skb);
		return NET_RX_DROP;
	}

	if (pvc->state.fecn != fh->fecn) {
#ifdef DEBUG_ECN
		printk(KERN_DEBUG "%s: DLCI %d FECN O%s\n", frad->name,
		       dlci, fh->fecn ? "N" : "FF");
#endif
		pvc->state.fecn ^= 1;
	}

	if (pvc->state.becn != fh->becn) {
#ifdef DEBUG_ECN
		printk(KERN_DEBUG "%s: DLCI %d BECN O%s\n", frad->name,
		       dlci, fh->becn ? "N" : "FF");
#endif
		pvc->state.becn ^= 1;
	}


	if ((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL) {
		frad->stats.rx_dropped++;
		return NET_RX_DROP;
	}

	if (data[3] == NLPID_IP) {
		skb_pull(skb, 4);  
		dev = pvc->main;
		skb->protocol = htons(ETH_P_IP);

	} else if (data[3] == NLPID_IPV6) {
		skb_pull(skb, 4);  
		dev = pvc->main;
		skb->protocol = htons(ETH_P_IPV6);

	} else if (skb->len > 10 && data[3] == FR_PAD &&
		   data[4] == NLPID_SNAP && data[5] == FR_PAD) {
		u16 oui = ntohs(*(__be16*)(data + 6));
		u16 pid = ntohs(*(__be16*)(data + 8));
		skb_pull(skb, 10);

		switch ((((u32)oui) << 16) | pid) {
		case ETH_P_ARP:  
		case ETH_P_IPX:
		case ETH_P_IP:	 
		case ETH_P_IPV6:
			dev = pvc->main;
			skb->protocol = htons(pid);
			break;

		case 0x80C20007:  
			if ((dev = pvc->ether) != NULL)
				skb->protocol = eth_type_trans(skb, dev);
			break;

		default:
			netdev_info(frad, "Unsupported protocol, OUI=%x PID=%x\n",
				    oui, pid);
			dev_kfree_skb_any(skb);
			return NET_RX_DROP;
		}
	} else {
		netdev_info(frad, "Unsupported protocol, NLPID=%x length=%i\n",
			    data[3], skb->len);
		dev_kfree_skb_any(skb);
		return NET_RX_DROP;
	}

	if (dev) {
		dev->stats.rx_packets++;  
		dev->stats.rx_bytes += skb->len;
		if (pvc->state.becn)
			dev->stats.rx_compressed++;
		skb->dev = dev;
		netif_rx(skb);
		return NET_RX_SUCCESS;
	} else {
		dev_kfree_skb_any(skb);
		return NET_RX_DROP;
	}

 rx_error:
	frad->stats.rx_errors++;  
	dev_kfree_skb_any(skb);
	return NET_RX_DROP;
}