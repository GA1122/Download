isdn_net_stat_callback(int idx, isdn_ctrl *c)
{
	isdn_net_dev *p = dev->st_netdev[idx];
	int cmd = c->command;

	if (p) {
		isdn_net_local *lp = p->local;
#ifdef CONFIG_ISDN_X25
		struct concap_proto *cprot = lp->netdev->cprot;
		struct concap_proto_ops *pops = cprot ? cprot->pops : NULL;
#endif
		switch (cmd) {
			case ISDN_STAT_BSENT:
				 
				if ((lp->flags & ISDN_NET_CONNECTED) &&
				    (!lp->dialstate)) {
					isdn_net_dec_frame_cnt(lp);
					lp->stats.tx_packets++;
					lp->stats.tx_bytes += c->parm.length;
				}
				return 1;
			case ISDN_STAT_DCONN:
				 
				switch (lp->dialstate) {
					case 4:
					case 7:
					case 8:
						lp->dialstate++;
						return 1;
					case 12:
						lp->dialstate = 5;
						return 1;
				}
				break;
			case ISDN_STAT_DHUP:
				 
#ifdef CONFIG_ISDN_X25
				 

				if( !(lp->flags & ISDN_NET_CONNECTED)
				    && pops && pops -> disconn_ind )
					pops -> disconn_ind(cprot);
#endif  
				if ((!lp->dialstate) && (lp->flags & ISDN_NET_CONNECTED)) {
					if (lp->p_encap == ISDN_NET_ENCAP_CISCOHDLCK)
						isdn_net_ciscohdlck_disconnected(lp);
#ifdef CONFIG_ISDN_PPP
					if (lp->p_encap == ISDN_NET_ENCAP_SYNCPPP)
						isdn_ppp_free(lp);
#endif
					isdn_net_lp_disconnected(lp);
					isdn_all_eaz(lp->isdn_device, lp->isdn_channel);
					printk(KERN_INFO "%s: remote hangup\n", p->dev->name);
					printk(KERN_INFO "%s: Chargesum is %d\n", p->dev->name,
					       lp->charge);
					isdn_net_unbind_channel(lp);
					return 1;
				}
				break;
#ifdef CONFIG_ISDN_X25
			case ISDN_STAT_BHUP:
				 
				 
				if( pops  &&  pops -> disconn_ind ){
						pops -> disconn_ind(cprot);
						return 1;
					}
				break;
#endif  
			case ISDN_STAT_BCONN:
				 
				isdn_net_zero_frame_cnt(lp);
				switch (lp->dialstate) {
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
					case 12:
						if (lp->dialstate <= 6) {
							dev->usage[idx] |= ISDN_USAGE_OUTGOING;
							isdn_info_update();
						} else
							dev->rx_netdev[idx] = p;
						lp->dialstate = 0;
						isdn_timer_ctrl(ISDN_TIMER_NETHANGUP, 1);
						if (lp->p_encap == ISDN_NET_ENCAP_CISCOHDLCK)
							isdn_net_ciscohdlck_connected(lp);
						if (lp->p_encap != ISDN_NET_ENCAP_SYNCPPP) {
							if (lp->master) {  
								isdn_net_dev *nd = ISDN_MASTER_PRIV(lp)->netdev;
								isdn_net_add_to_bundle(nd, lp);
							}
						}
						printk(KERN_INFO "isdn_net: %s connected\n", p->dev->name);
						 
						lp->chargetime = jiffies;

						 
						lp->dialstarted = 0;
						lp->dialwait_timer = 0;

#ifdef CONFIG_ISDN_PPP
						if (lp->p_encap == ISDN_NET_ENCAP_SYNCPPP)
							isdn_ppp_wakeup_daemon(lp);
#endif
#ifdef CONFIG_ISDN_X25
						 
						if( pops )
							if( pops->connect_ind)
								pops->connect_ind(cprot);
#endif  
						 
						if (lp->p_encap != ISDN_NET_ENCAP_SYNCPPP)
							isdn_net_device_wake_queue(lp);
						return 1;
				}
				break;
			case ISDN_STAT_NODCH:
				 
				if (lp->dialstate == 4) {
					lp->dialstate--;
					return 1;
				}
				break;
			case ISDN_STAT_CINF:
				 
				lp->charge++;
				if (lp->hupflags & ISDN_HAVECHARGE) {
					lp->hupflags &= ~ISDN_WAITCHARGE;
					lp->chargeint = jiffies - lp->chargetime - (2 * HZ);
				}
				if (lp->hupflags & ISDN_WAITCHARGE)
					lp->hupflags |= ISDN_HAVECHARGE;
				lp->chargetime = jiffies;
				printk(KERN_DEBUG "isdn_net: Got CINF chargetime of %s now %lu\n",
				       p->dev->name, lp->chargetime);
				return 1;
		}
	}
	return 0;
}
