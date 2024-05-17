 void fib_del_ifaddr(struct in_ifaddr *ifa, struct in_ifaddr *iprim)
 {
 	struct in_device *in_dev = ifa->ifa_dev;
 	struct net_device *dev = in_dev->dev;
 	struct in_ifaddr *ifa1;
 	struct in_ifaddr *prim = ifa, *prim1 = NULL;
 	__be32 brd = ifa->ifa_address | ~ifa->ifa_mask;
 	__be32 any = ifa->ifa_address & ifa->ifa_mask;
 #define LOCAL_OK	1
 #define BRD_OK		2
 #define BRD0_OK		4
 #define BRD1_OK		8
 	unsigned int ok = 0;
 	int subnet = 0;		 
 	int gone = 1;		 
 	int same_prefsrc = 0;	 
 
 	if (ifa->ifa_flags & IFA_F_SECONDARY) {
 		prim = inet_ifa_byprefix(in_dev, any, ifa->ifa_mask);
 		if (!prim) {
 			pr_warn("%s: bug: prim == NULL\n", __func__);
 			return;
 		}
 		if (iprim && iprim != prim) {
 			pr_warn("%s: bug: iprim != prim\n", __func__);
 			return;
 		}
 	} else if (!ipv4_is_zeronet(any) &&
 		   (any != ifa->ifa_local || ifa->ifa_prefixlen < 32)) {
 		if (!(ifa->ifa_flags & IFA_F_NOPREFIXROUTE))
 			fib_magic(RTM_DELROUTE,
 				  dev->flags & IFF_LOOPBACK ? RTN_LOCAL : RTN_UNICAST,
 				  any, ifa->ifa_prefixlen, prim);
  		subnet = 1;
  	}
  
// 	if (in_dev->dead)
// 		goto no_promotions;
// 
  	 
 
 	for (ifa1 = in_dev->ifa_list; ifa1; ifa1 = ifa1->ifa_next) {
 		if (ifa1 == ifa) {
 			 
 			gone = 0;
 			continue;
 		}
 		 
 		if (iprim && ifa1->ifa_mask == iprim->ifa_mask &&
 		    inet_ifa_match(ifa1->ifa_address, iprim))
 			continue;
 
 		 
 		if (ifa1->ifa_flags & IFA_F_SECONDARY) {
 			 
 			if (ifa1->ifa_mask == prim->ifa_mask &&
 			    inet_ifa_match(ifa1->ifa_address, prim))
 				prim1 = prim;
 			else {
 				 
 				if (!same_prefsrc)
 					continue;
 				 
 				if (!prim1 ||
 				    ifa1->ifa_mask != prim1->ifa_mask ||
 				    !inet_ifa_match(ifa1->ifa_address, prim1))
 					prim1 = inet_ifa_byprefix(in_dev,
 							ifa1->ifa_address,
 							ifa1->ifa_mask);
 				if (!prim1)
 					continue;
 				if (prim1->ifa_local != prim->ifa_local)
 					continue;
 			}
 		} else {
 			if (prim->ifa_local != ifa1->ifa_local)
 				continue;
 			prim1 = ifa1;
 			if (prim != prim1)
 				same_prefsrc = 1;
 		}
 		if (ifa->ifa_local == ifa1->ifa_local)
 			ok |= LOCAL_OK;
 		if (ifa->ifa_broadcast == ifa1->ifa_broadcast)
 			ok |= BRD_OK;
 		if (brd == ifa1->ifa_broadcast)
 			ok |= BRD1_OK;
 		if (any == ifa1->ifa_broadcast)
 			ok |= BRD0_OK;
 		 
 		if (prim1 == ifa1 && ifa1->ifa_prefixlen < 31) {
 			__be32 brd1 = ifa1->ifa_address | ~ifa1->ifa_mask;
 			__be32 any1 = ifa1->ifa_address & ifa1->ifa_mask;
 
 			if (!ipv4_is_zeronet(any1)) {
 				if (ifa->ifa_broadcast == brd1 ||
 				    ifa->ifa_broadcast == any1)
 					ok |= BRD_OK;
 				if (brd == brd1 || brd == any1)
 					ok |= BRD1_OK;
 				if (any == brd1 || any == any1)
 					ok |= BRD0_OK;
 			}
  		}
  	}
  
// no_promotions:
  	if (!(ok & BRD_OK))
  		fib_magic(RTM_DELROUTE, RTN_BROADCAST, ifa->ifa_broadcast, 32, prim);
  	if (subnet && ifa->ifa_prefixlen < 31) {
 		if (!(ok & BRD1_OK))
 			fib_magic(RTM_DELROUTE, RTN_BROADCAST, brd, 32, prim);
 		if (!(ok & BRD0_OK))
 			fib_magic(RTM_DELROUTE, RTN_BROADCAST, any, 32, prim);
 	}
 	if (!(ok & LOCAL_OK)) {
 		unsigned int addr_type;
 
 		fib_magic(RTM_DELROUTE, RTN_LOCAL, ifa->ifa_local, 32, prim);
 
 		 
 		addr_type = inet_addr_type_dev_table(dev_net(dev), dev,
 						     ifa->ifa_local);
 		if (gone && addr_type != RTN_LOCAL) {
 			 
 			if (fib_sync_down_addr(dev_net(dev), ifa->ifa_local))
 				fib_flush(dev_net(dev));
 		}
 	}
 #undef LOCAL_OK
 #undef BRD_OK
 #undef BRD0_OK
 #undef BRD1_OK
 }