isdn_net_setcfg(isdn_net_ioctl_cfg * cfg)
{
	isdn_net_dev *p = isdn_net_findif(cfg->name);
	ulong features;
	int i;
	int drvidx;
	int chidx;
	char drvid[25];

	if (p) {
		isdn_net_local *lp = p->local;

		 
		features = ((1 << cfg->l2_proto) << ISDN_FEATURE_L2_SHIFT) |
			((1 << cfg->l3_proto) << ISDN_FEATURE_L3_SHIFT);
		for (i = 0; i < ISDN_MAX_DRIVERS; i++)
			if (dev->drv[i])
				if ((dev->drv[i]->interface->features & features) == features)
					break;
		if (i == ISDN_MAX_DRIVERS) {
			printk(KERN_WARNING "isdn_net: No driver with selected features\n");
			return -ENODEV;
		}
		if (lp->p_encap != cfg->p_encap){
#ifdef CONFIG_ISDN_X25
			struct concap_proto * cprot = p -> cprot;
#endif
			if (isdn_net_device_started(p)) {
				printk(KERN_WARNING "%s: cannot change encap when if is up\n",
				       p->dev->name);
				return -EBUSY;
			}
#ifdef CONFIG_ISDN_X25
			if( cprot && cprot -> pops )
				cprot -> pops -> proto_del ( cprot );
			p -> cprot = NULL;
			lp -> dops = NULL;
			 
			switch ( cfg -> p_encap ){
			case ISDN_NET_ENCAP_X25IFACE:
				lp -> dops = &isdn_concap_reliable_dl_dops;
			}
			 
			p -> cprot = isdn_concap_new( cfg -> p_encap );
			 
			 
#endif
		}
		switch ( cfg->p_encap ) {
		case ISDN_NET_ENCAP_SYNCPPP:
#ifndef CONFIG_ISDN_PPP
			printk(KERN_WARNING "%s: SyncPPP support not configured\n",
			       p->dev->name);
			return -EINVAL;
#else
			p->dev->type = ARPHRD_PPP;	 
			p->dev->addr_len = 0;
#endif
			break;
		case ISDN_NET_ENCAP_X25IFACE:
#ifndef CONFIG_ISDN_X25
			printk(KERN_WARNING "%s: isdn-x25 support not configured\n",
			       p->dev->name);
			return -EINVAL;
#else
			p->dev->type = ARPHRD_X25;	 
			p->dev->addr_len = 0;
#endif
			break;
		case ISDN_NET_ENCAP_CISCOHDLCK:
			break;
		default:
			if( cfg->p_encap >= 0 &&
			    cfg->p_encap <= ISDN_NET_ENCAP_MAX_ENCAP )
				break;
			printk(KERN_WARNING
			       "%s: encapsulation protocol %d not supported\n",
			       p->dev->name, cfg->p_encap);
			return -EINVAL;
		}
		if (strlen(cfg->drvid)) {
			 
			char *c,
			*e;

			drvidx = -1;
			chidx = -1;
			strcpy(drvid, cfg->drvid);
			if ((c = strchr(drvid, ','))) {
				 
				chidx = (int) simple_strtoul(c + 1, &e, 10);
				if (e == c)
					chidx = -1;
				*c = '\0';
			}
			for (i = 0; i < ISDN_MAX_DRIVERS; i++)
				 
				if (!(strcmp(dev->drvid[i], drvid))) {
					drvidx = i;
					break;
				}
			if ((drvidx == -1) || (chidx == -1))
				 
				return -ENODEV;
		} else {
			 
			drvidx = lp->pre_device;
			chidx = lp->pre_channel;
		}
		if (cfg->exclusive > 0) {
			unsigned long flags;

			 
			spin_lock_irqsave(&dev->lock, flags);
			if ((i = isdn_get_free_channel(ISDN_USAGE_NET,
				lp->l2_proto, lp->l3_proto, drvidx,
				chidx, lp->msn)) < 0) {
				 
				lp->exclusive = -1;
				spin_unlock_irqrestore(&dev->lock, flags);
				return -EBUSY;
			}
			 
			dev->usage[i] = ISDN_USAGE_EXCLUSIVE;
			isdn_info_update();
			spin_unlock_irqrestore(&dev->lock, flags);
			lp->exclusive = i;
		} else {
			 
			lp->exclusive = -1;
			if ((lp->pre_device != -1) && (cfg->exclusive == -1)) {
				isdn_unexclusive_channel(lp->pre_device, lp->pre_channel);
				isdn_free_channel(lp->pre_device, lp->pre_channel, ISDN_USAGE_NET);
				drvidx = -1;
				chidx = -1;
			}
		}
		strlcpy(lp->msn, cfg->eaz, sizeof(lp->msn));
		lp->pre_device = drvidx;
		lp->pre_channel = chidx;
		lp->onhtime = cfg->onhtime;
		lp->charge = cfg->charge;
		lp->l2_proto = cfg->l2_proto;
		lp->l3_proto = cfg->l3_proto;
		lp->cbdelay = cfg->cbdelay;
		lp->dialmax = cfg->dialmax;
		lp->triggercps = cfg->triggercps;
		lp->slavedelay = cfg->slavedelay * HZ;
		lp->pppbind = cfg->pppbind;
		lp->dialtimeout = cfg->dialtimeout >= 0 ? cfg->dialtimeout * HZ : -1;
		lp->dialwait = cfg->dialwait * HZ;
		if (cfg->secure)
			lp->flags |= ISDN_NET_SECURE;
		else
			lp->flags &= ~ISDN_NET_SECURE;
		if (cfg->cbhup)
			lp->flags |= ISDN_NET_CBHUP;
		else
			lp->flags &= ~ISDN_NET_CBHUP;
		switch (cfg->callback) {
			case 0:
				lp->flags &= ~(ISDN_NET_CALLBACK | ISDN_NET_CBOUT);
				break;
			case 1:
				lp->flags |= ISDN_NET_CALLBACK;
				lp->flags &= ~ISDN_NET_CBOUT;
				break;
			case 2:
				lp->flags |= ISDN_NET_CBOUT;
				lp->flags &= ~ISDN_NET_CALLBACK;
				break;
		}
		lp->flags &= ~ISDN_NET_DIALMODE_MASK;	 
		if (cfg->dialmode && !(cfg->dialmode & ISDN_NET_DIALMODE_MASK)) {
			 
			printk(KERN_WARNING
			     "Old isdnctrl version detected! Please update.\n");
			lp->flags |= ISDN_NET_DM_OFF;  
		}
		else {
			lp->flags |= cfg->dialmode;   
		}
		if (cfg->chargehup)
			lp->hupflags |= ISDN_CHARGEHUP;
		else
			lp->hupflags &= ~ISDN_CHARGEHUP;
		if (cfg->ihup)
			lp->hupflags |= ISDN_INHUP;
		else
			lp->hupflags &= ~ISDN_INHUP;
		if (cfg->chargeint > 10) {
			lp->hupflags |= ISDN_CHARGEHUP | ISDN_HAVECHARGE | ISDN_MANCHARGE;
			lp->chargeint = cfg->chargeint * HZ;
		}
		if (cfg->p_encap != lp->p_encap) {
			if (cfg->p_encap == ISDN_NET_ENCAP_RAWIP) {
				p->dev->header_ops = NULL;
				p->dev->flags = IFF_NOARP|IFF_POINTOPOINT;
			} else {
				p->dev->header_ops = &isdn_header_ops;
				if (cfg->p_encap == ISDN_NET_ENCAP_ETHER)
					p->dev->flags = IFF_BROADCAST | IFF_MULTICAST;
				else
					p->dev->flags = IFF_NOARP|IFF_POINTOPOINT;
			}
		}
		lp->p_encap = cfg->p_encap;
		return 0;
	}
	return -ENODEV;
}
