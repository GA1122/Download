isdn_net_find_icall(int di, int ch, int idx, setup_parm *setup)
{
	char *eaz;
	int si1;
	int si2;
	int ematch;
	int wret;
	int swapped;
	int sidx = 0;
	u_long flags;
	isdn_net_dev *p;
	isdn_net_phone *n;
	char nr[ISDN_MSNLEN];
	char *my_eaz;

	 
	if (!setup->phone[0]) {
		nr[0] = '0';
		nr[1] = '\0';
		printk(KERN_INFO "isdn_net: Incoming call without OAD, assuming '0'\n");
	} else
		strlcpy(nr, setup->phone, ISDN_MSNLEN);
	si1 = (int) setup->si1;
	si2 = (int) setup->si2;
	if (!setup->eazmsn[0]) {
		printk(KERN_WARNING "isdn_net: Incoming call without CPN, assuming '0'\n");
		eaz = "0";
	} else
		eaz = setup->eazmsn;
	if (dev->net_verbose > 1)
		printk(KERN_INFO "isdn_net: call from %s,%d,%d -> %s\n", nr, si1, si2, eaz);
	 
	if ((si1 != 7) && (si1 != 1)) {
		if (dev->net_verbose > 1)
			printk(KERN_INFO "isdn_net: Service-Indicator not 1 or 7, ignored\n");
		return 0;
	}
	n = (isdn_net_phone *) 0;
	p = dev->netdev;
	ematch = wret = swapped = 0;
#ifdef ISDN_DEBUG_NET_ICALL
	printk(KERN_DEBUG "n_fi: di=%d ch=%d idx=%d usg=%d\n", di, ch, idx,
		dev->usage[idx]);
#endif
	while (p) {
		int matchret;
		isdn_net_local *lp = p->local;

		 
		switch (swapped) {
			case 2:
				isdn_net_swap_usage(idx, sidx);
				 
			case 1:
				isdn_net_swapbind(di);
				break;
		}
		swapped = 0;
                 
                my_eaz = isdn_map_eaz2msn(lp->msn, di);
                if (si1 == 1) {  
                        if (*my_eaz == 'v' || *my_eaz == 'V' ||
			    *my_eaz == 'b' || *my_eaz == 'B')
                                my_eaz++;  
                        else
                                my_eaz = NULL;  
                } else {  
                        if (*my_eaz == 'b' || *my_eaz == 'B')
                                my_eaz++;  
                }
                if (my_eaz)
                        matchret = isdn_msncmp(eaz, my_eaz);
                else
                        matchret = 1;
                if (!matchret)
                        ematch = 1;

		 
		if (matchret > wret)
			wret = matchret;
#ifdef ISDN_DEBUG_NET_ICALL
		printk(KERN_DEBUG "n_fi: if='%s', l.msn=%s, l.flags=%d, l.dstate=%d\n",
		       p->dev->name, lp->msn, lp->flags, lp->dialstate);
#endif
		if ((!matchret) &&                                         
		    (((!(lp->flags & ISDN_NET_CONNECTED)) &&               
		      (USG_NONE(dev->usage[idx]))) ||                      
		     ((((lp->dialstate == 4) || (lp->dialstate == 12)) &&  
		       (!(lp->flags & ISDN_NET_CALLBACK)))                 
		     )))
			 {
#ifdef ISDN_DEBUG_NET_ICALL
			printk(KERN_DEBUG "n_fi: match1, pdev=%d pch=%d\n",
			       lp->pre_device, lp->pre_channel);
#endif
			if (dev->usage[idx] & ISDN_USAGE_EXCLUSIVE) {
				if ((lp->pre_channel != ch) ||
				    (lp->pre_device != di)) {
					 
					if (ch == 0) {
						sidx = isdn_dc2minor(di, 1);
#ifdef ISDN_DEBUG_NET_ICALL
						printk(KERN_DEBUG "n_fi: ch is 0\n");
#endif
						if (USG_NONE(dev->usage[sidx])) {
							 
							if (dev->usage[sidx] & ISDN_USAGE_EXCLUSIVE) {
#ifdef ISDN_DEBUG_NET_ICALL
								printk(KERN_DEBUG "n_fi: 2nd channel is down and bound\n");
#endif
								 
								if ((lp->pre_device == di) &&
								    (lp->pre_channel == 1)) {
									isdn_net_swapbind(di);
									swapped = 1;
								} else {
									 
									p = (isdn_net_dev *) p->next;
									continue;
								}
							} else {
#ifdef ISDN_DEBUG_NET_ICALL
								printk(KERN_DEBUG "n_fi: 2nd channel is down and unbound\n");
#endif
								 
								isdn_net_swap_usage(idx, sidx);
								isdn_net_swapbind(di);
								swapped = 2;
							}
							 
#ifdef ISDN_DEBUG_NET_ICALL
							printk(KERN_DEBUG "n_fi: final check\n");
#endif
							if ((dev->usage[idx] & ISDN_USAGE_EXCLUSIVE) &&
							    ((lp->pre_channel != ch) ||
							     (lp->pre_device != di))) {
#ifdef ISDN_DEBUG_NET_ICALL
								printk(KERN_DEBUG "n_fi: final check failed\n");
#endif
								p = (isdn_net_dev *) p->next;
								continue;
							}
						}
					} else {
						 
#ifdef ISDN_DEBUG_NET_ICALL
						printk(KERN_DEBUG "n_fi: already on 2nd channel\n");
#endif
					}
				}
			}
#ifdef ISDN_DEBUG_NET_ICALL
			printk(KERN_DEBUG "n_fi: match2\n");
#endif
			n = lp->phone[0];
			if (lp->flags & ISDN_NET_SECURE) {
				while (n) {
					if (!isdn_msncmp(nr, n->num))
						break;
					n = (isdn_net_phone *) n->next;
				}
			}
			if (n || (!(lp->flags & ISDN_NET_SECURE))) {
#ifdef ISDN_DEBUG_NET_ICALL
				printk(KERN_DEBUG "n_fi: match3\n");
#endif
				 

				 
				if (ISDN_NET_DIALMODE(*lp) == ISDN_NET_DM_OFF) {
					printk(KERN_INFO "incoming call, interface %s `stopped' -> rejected\n",
					       p->dev->name);
					return 3;
				}
				 
				if (!isdn_net_device_started(p)) {
					printk(KERN_INFO "%s: incoming call, interface down -> rejected\n",
					       p->dev->name);
					return 3;
				}
				 
				if (lp->master) {
					isdn_net_local *mlp = ISDN_MASTER_PRIV(lp);
					printk(KERN_DEBUG "ICALLslv: %s\n", p->dev->name);
					printk(KERN_DEBUG "master=%s\n", lp->master->name);
					if (mlp->flags & ISDN_NET_CONNECTED) {
						printk(KERN_DEBUG "master online\n");
						 
						while (mlp->slave) {
							if (ISDN_SLAVE_PRIV(mlp) == lp)
								break;
							mlp = ISDN_SLAVE_PRIV(mlp);
						}
					} else
						printk(KERN_DEBUG "master offline\n");
					 
					printk(KERN_DEBUG "mlpf: %d\n", mlp->flags & ISDN_NET_CONNECTED);
					if (!(mlp->flags & ISDN_NET_CONNECTED)) {
						p = (isdn_net_dev *) p->next;
						continue;
					}
				} 
				if (lp->flags & ISDN_NET_CALLBACK) {
					int chi;
					 
					if (ISDN_NET_DIALMODE(*lp) == ISDN_NET_DM_OFF) {
						printk(KERN_INFO "incoming call for callback, interface %s `off' -> rejected\n",
						       p->dev->name);
						return 3;
					}
					printk(KERN_DEBUG "%s: call from %s -> %s, start callback\n",
					       p->dev->name, nr, eaz);
					if (lp->phone[1]) {
						 
						spin_lock_irqsave(&dev->lock, flags);
						if ((chi = 
							isdn_get_free_channel(
								ISDN_USAGE_NET,
								lp->l2_proto,
								lp->l3_proto,
							  	lp->pre_device,
						 		lp->pre_channel,
						 		lp->msn)
								) < 0) {

							printk(KERN_WARNING "isdn_net_find_icall: No channel for %s\n",
								p->dev->name);
							spin_unlock_irqrestore(&dev->lock, flags);
							return 0;
						}
						 
						lp->dtimer = 0;
						lp->dialstate = 11;
						 
						isdn_net_bind_channel(lp, chi);
#ifdef CONFIG_ISDN_PPP
						if (lp->p_encap == ISDN_NET_ENCAP_SYNCPPP)
							if (isdn_ppp_bind(lp) < 0) {
								spin_unlock_irqrestore(&dev->lock, flags);
								isdn_net_unbind_channel(lp);
								return 0;
							}
#endif
						spin_unlock_irqrestore(&dev->lock, flags);
						 
						return (lp->flags & ISDN_NET_CBHUP) ? 2 : 4;
					} else
						printk(KERN_WARNING "isdn_net: %s: No phone number\n",
							p->dev->name);
					return 0;
				} else {
					printk(KERN_DEBUG "%s: call from %s -> %s accepted\n",
						p->dev->name, nr, eaz);
					 
					if ((lp->dialstate == 4) || (lp->dialstate == 12)) {
#ifdef CONFIG_ISDN_PPP
						if (lp->p_encap == ISDN_NET_ENCAP_SYNCPPP)
							isdn_ppp_free(lp);
#endif
						isdn_net_lp_disconnected(lp);
						isdn_free_channel(lp->isdn_device, lp->isdn_channel,
							 ISDN_USAGE_NET);
					}
					spin_lock_irqsave(&dev->lock, flags);
					dev->usage[idx] &= ISDN_USAGE_EXCLUSIVE;
					dev->usage[idx] |= ISDN_USAGE_NET;
					strcpy(dev->num[idx], nr);
					isdn_info_update();
					dev->st_netdev[idx] = lp->netdev;
					lp->isdn_device = di;
					lp->isdn_channel = ch;
					lp->ppp_slot = -1;
					lp->flags |= ISDN_NET_CONNECTED;
					lp->dialstate = 7;
					lp->dtimer = 0;
					lp->outgoing = 0;
					lp->huptimer = 0;
					lp->hupflags |= ISDN_WAITCHARGE;
					lp->hupflags &= ~ISDN_HAVECHARGE;
#ifdef CONFIG_ISDN_PPP
					if (lp->p_encap == ISDN_NET_ENCAP_SYNCPPP) {
						if (isdn_ppp_bind(lp) < 0) {
							isdn_net_unbind_channel(lp);
							spin_unlock_irqrestore(&dev->lock, flags);
							return 0;
						}
					}
#endif
					spin_unlock_irqrestore(&dev->lock, flags);
					return 1;
				}
			}
		}
		p = (isdn_net_dev *) p->next;
	}
	 
	if (!ematch || dev->net_verbose)
		printk(KERN_INFO "isdn_net: call from %s -> %d %s ignored\n", nr, di, eaz);
	return (wret == 2)?5:0;
}