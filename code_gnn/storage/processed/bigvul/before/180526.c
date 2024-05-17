 static void __inet_del_ifa(struct in_device *in_dev, struct in_ifaddr **ifap,
 			 int destroy, struct nlmsghdr *nlh, u32 portid)
 {
 	struct in_ifaddr *promote = NULL;
 	struct in_ifaddr *ifa, *ifa1 = *ifap;
 	struct in_ifaddr *last_prim = in_dev->ifa_list;
 	struct in_ifaddr *prev_prom = NULL;
 	int do_promote = IN_DEV_PROMOTE_SECONDARIES(in_dev);
  
  	ASSERT_RTNL();
  
// 	if (in_dev->dead)
// 		goto no_promotions;
// 
  	 
 
 	if (!(ifa1->ifa_flags & IFA_F_SECONDARY)) {
 		struct in_ifaddr **ifap1 = &ifa1->ifa_next;
 
 		while ((ifa = *ifap1) != NULL) {
 			if (!(ifa->ifa_flags & IFA_F_SECONDARY) &&
 			    ifa1->ifa_scope <= ifa->ifa_scope)
 				last_prim = ifa;
 
 			if (!(ifa->ifa_flags & IFA_F_SECONDARY) ||
 			    ifa1->ifa_mask != ifa->ifa_mask ||
 			    !inet_ifa_match(ifa1->ifa_address, ifa)) {
 				ifap1 = &ifa->ifa_next;
 				prev_prom = ifa;
 				continue;
 			}
 
 			if (!do_promote) {
 				inet_hash_remove(ifa);
 				*ifap1 = ifa->ifa_next;
 
 				rtmsg_ifa(RTM_DELADDR, ifa, nlh, portid);
 				blocking_notifier_call_chain(&inetaddr_chain,
 						NETDEV_DOWN, ifa);
 				inet_free_ifa(ifa);
 			} else {
 				promote = ifa;
 				break;
 			}
 		}
 	}
 
 	 
 	for (ifa = promote; ifa; ifa = ifa->ifa_next) {
 		if (ifa1->ifa_mask == ifa->ifa_mask &&
 		    inet_ifa_match(ifa1->ifa_address, ifa))
  			fib_del_ifaddr(ifa, ifa1);
  	}
  
// no_promotions:
  	 
  
  	*ifap = ifa1->ifa_next;
 	inet_hash_remove(ifa1);
 
 	 
 
 	 
 	rtmsg_ifa(RTM_DELADDR, ifa1, nlh, portid);
 	blocking_notifier_call_chain(&inetaddr_chain, NETDEV_DOWN, ifa1);
 
 	if (promote) {
 		struct in_ifaddr *next_sec = promote->ifa_next;
 
 		if (prev_prom) {
 			prev_prom->ifa_next = promote->ifa_next;
 			promote->ifa_next = last_prim->ifa_next;
 			last_prim->ifa_next = promote;
 		}
 
 		promote->ifa_flags &= ~IFA_F_SECONDARY;
 		rtmsg_ifa(RTM_NEWADDR, promote, nlh, portid);
 		blocking_notifier_call_chain(&inetaddr_chain,
 				NETDEV_UP, promote);
 		for (ifa = next_sec; ifa; ifa = ifa->ifa_next) {
 			if (ifa1->ifa_mask != ifa->ifa_mask ||
 			    !inet_ifa_match(ifa1->ifa_address, ifa))
 					continue;
 			fib_add_ifaddr(ifa);
 		}
 
 	}
 	if (destroy)
 		inet_free_ifa(ifa1);
 }