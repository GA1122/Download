 static int do_ip_setsockopt(struct sock *sk, int level,
 			    int optname, char __user *optval, unsigned int optlen)
 {
 	struct inet_sock *inet = inet_sk(sk);
 	int val = 0, err;
 
 	if (((1<<optname) & ((1<<IP_PKTINFO) | (1<<IP_RECVTTL) |
 			     (1<<IP_RECVOPTS) | (1<<IP_RECVTOS) |
 			     (1<<IP_RETOPTS) | (1<<IP_TOS) |
 			     (1<<IP_TTL) | (1<<IP_HDRINCL) |
 			     (1<<IP_MTU_DISCOVER) | (1<<IP_RECVERR) |
 			     (1<<IP_ROUTER_ALERT) | (1<<IP_FREEBIND) |
 			     (1<<IP_PASSSEC) | (1<<IP_TRANSPARENT) |
 			     (1<<IP_MINTTL) | (1<<IP_NODEFRAG))) ||
 	    optname == IP_MULTICAST_TTL ||
 	    optname == IP_MULTICAST_ALL ||
 	    optname == IP_MULTICAST_LOOP ||
 	    optname == IP_RECVORIGDSTADDR) {
 		if (optlen >= sizeof(int)) {
 			if (get_user(val, (int __user *) optval))
 				return -EFAULT;
 		} else if (optlen >= sizeof(char)) {
 			unsigned char ucval;
 
 			if (get_user(ucval, (unsigned char __user *) optval))
 				return -EFAULT;
 			val = (int) ucval;
 		}
 	}
 
 	 
 
 	if (ip_mroute_opt(optname))
 		return ip_mroute_setsockopt(sk, optname, optval, optlen);
 
 	err = 0;
 	lock_sock(sk);
 
  	switch (optname) {
  	case IP_OPTIONS:
  	{
		struct ip_options *opt = NULL;
// 		struct ip_options_rcu *old, *opt = NULL;
// 
  		if (optlen > 40)
  			goto e_inval;
  		err = ip_options_get_from_user(sock_net(sk), &opt,
  					       optval, optlen);
  		if (err)
  			break;
// 		old = rcu_dereference_protected(inet->inet_opt,
// 						sock_owned_by_user(sk));
  		if (inet->is_icsk) {
  			struct inet_connection_sock *icsk = inet_csk(sk);
  #if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
 			if (sk->sk_family == PF_INET ||
 			    (!((1 << sk->sk_state) &
  			       (TCPF_LISTEN | TCPF_CLOSE)) &&
  			     inet->inet_daddr != LOOPBACK4_IPV6)) {
  #endif
				if (inet->opt)
					icsk->icsk_ext_hdr_len -= inet->opt->optlen;
// 				if (old)
// 					icsk->icsk_ext_hdr_len -= old->opt.optlen;
  				if (opt)
					icsk->icsk_ext_hdr_len += opt->optlen;
// 					icsk->icsk_ext_hdr_len += opt->opt.optlen;
  				icsk->icsk_sync_mss(sk, icsk->icsk_pmtu_cookie);
  #if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
  			}
  #endif
  		}
		opt = xchg(&inet->opt, opt);
		kfree(opt);
// 		rcu_assign_pointer(inet->inet_opt, opt);
// 		if (old)
// 			call_rcu(&old->rcu, opt_kfree_rcu);
  		break;
  	}
  	case IP_PKTINFO:
 		if (val)
 			inet->cmsg_flags |= IP_CMSG_PKTINFO;
 		else
 			inet->cmsg_flags &= ~IP_CMSG_PKTINFO;
 		break;
 	case IP_RECVTTL:
 		if (val)
 			inet->cmsg_flags |=  IP_CMSG_TTL;
 		else
 			inet->cmsg_flags &= ~IP_CMSG_TTL;
 		break;
 	case IP_RECVTOS:
 		if (val)
 			inet->cmsg_flags |=  IP_CMSG_TOS;
 		else
 			inet->cmsg_flags &= ~IP_CMSG_TOS;
 		break;
 	case IP_RECVOPTS:
 		if (val)
 			inet->cmsg_flags |=  IP_CMSG_RECVOPTS;
 		else
 			inet->cmsg_flags &= ~IP_CMSG_RECVOPTS;
 		break;
 	case IP_RETOPTS:
 		if (val)
 			inet->cmsg_flags |= IP_CMSG_RETOPTS;
 		else
 			inet->cmsg_flags &= ~IP_CMSG_RETOPTS;
 		break;
 	case IP_PASSSEC:
 		if (val)
 			inet->cmsg_flags |= IP_CMSG_PASSSEC;
 		else
 			inet->cmsg_flags &= ~IP_CMSG_PASSSEC;
 		break;
 	case IP_RECVORIGDSTADDR:
 		if (val)
 			inet->cmsg_flags |= IP_CMSG_ORIGDSTADDR;
 		else
 			inet->cmsg_flags &= ~IP_CMSG_ORIGDSTADDR;
 		break;
 	case IP_TOS:	 
 		if (sk->sk_type == SOCK_STREAM) {
 			val &= ~3;
 			val |= inet->tos & 3;
 		}
 		if (inet->tos != val) {
 			inet->tos = val;
 			sk->sk_priority = rt_tos2priority(val);
 			sk_dst_reset(sk);
 		}
 		break;
 	case IP_TTL:
 		if (optlen < 1)
 			goto e_inval;
 		if (val != -1 && (val < 0 || val > 255))
 			goto e_inval;
 		inet->uc_ttl = val;
 		break;
 	case IP_HDRINCL:
 		if (sk->sk_type != SOCK_RAW) {
 			err = -ENOPROTOOPT;
 			break;
 		}
 		inet->hdrincl = val ? 1 : 0;
 		break;
 	case IP_NODEFRAG:
 		if (sk->sk_type != SOCK_RAW) {
 			err = -ENOPROTOOPT;
 			break;
 		}
 		inet->nodefrag = val ? 1 : 0;
 		break;
 	case IP_MTU_DISCOVER:
 		if (val < IP_PMTUDISC_DONT || val > IP_PMTUDISC_PROBE)
 			goto e_inval;
 		inet->pmtudisc = val;
 		break;
 	case IP_RECVERR:
 		inet->recverr = !!val;
 		if (!val)
 			skb_queue_purge(&sk->sk_error_queue);
 		break;
 	case IP_MULTICAST_TTL:
 		if (sk->sk_type == SOCK_STREAM)
 			goto e_inval;
 		if (optlen < 1)
 			goto e_inval;
 		if (val == -1)
 			val = 1;
 		if (val < 0 || val > 255)
 			goto e_inval;
 		inet->mc_ttl = val;
 		break;
 	case IP_MULTICAST_LOOP:
 		if (optlen < 1)
 			goto e_inval;
 		inet->mc_loop = !!val;
 		break;
 	case IP_MULTICAST_IF:
 	{
 		struct ip_mreqn mreq;
 		struct net_device *dev = NULL;
 
 		if (sk->sk_type == SOCK_STREAM)
 			goto e_inval;
 		 
 
 		if (optlen < sizeof(struct in_addr))
 			goto e_inval;
 
 		err = -EFAULT;
 		if (optlen >= sizeof(struct ip_mreqn)) {
 			if (copy_from_user(&mreq, optval, sizeof(mreq)))
 				break;
 		} else {
 			memset(&mreq, 0, sizeof(mreq));
 			if (optlen >= sizeof(struct in_addr) &&
 			    copy_from_user(&mreq.imr_address, optval,
 					   sizeof(struct in_addr)))
 				break;
 		}
 
 		if (!mreq.imr_ifindex) {
 			if (mreq.imr_address.s_addr == htonl(INADDR_ANY)) {
 				inet->mc_index = 0;
 				inet->mc_addr  = 0;
 				err = 0;
 				break;
 			}
 			dev = ip_dev_find(sock_net(sk), mreq.imr_address.s_addr);
 			if (dev)
 				mreq.imr_ifindex = dev->ifindex;
 		} else
 			dev = dev_get_by_index(sock_net(sk), mreq.imr_ifindex);
 
 
 		err = -EADDRNOTAVAIL;
 		if (!dev)
 			break;
 		dev_put(dev);
 
 		err = -EINVAL;
 		if (sk->sk_bound_dev_if &&
 		    mreq.imr_ifindex != sk->sk_bound_dev_if)
 			break;
 
 		inet->mc_index = mreq.imr_ifindex;
 		inet->mc_addr  = mreq.imr_address.s_addr;
 		err = 0;
 		break;
 	}
 
 	case IP_ADD_MEMBERSHIP:
 	case IP_DROP_MEMBERSHIP:
 	{
 		struct ip_mreqn mreq;
 
 		err = -EPROTO;
 		if (inet_sk(sk)->is_icsk)
 			break;
 
 		if (optlen < sizeof(struct ip_mreq))
 			goto e_inval;
 		err = -EFAULT;
 		if (optlen >= sizeof(struct ip_mreqn)) {
 			if (copy_from_user(&mreq, optval, sizeof(mreq)))
 				break;
 		} else {
 			memset(&mreq, 0, sizeof(mreq));
 			if (copy_from_user(&mreq, optval, sizeof(struct ip_mreq)))
 				break;
 		}
 
 		if (optname == IP_ADD_MEMBERSHIP)
 			err = ip_mc_join_group(sk, &mreq);
 		else
 			err = ip_mc_leave_group(sk, &mreq);
 		break;
 	}
 	case IP_MSFILTER:
 	{
 		struct ip_msfilter *msf;
 
 		if (optlen < IP_MSFILTER_SIZE(0))
 			goto e_inval;
 		if (optlen > sysctl_optmem_max) {
 			err = -ENOBUFS;
 			break;
 		}
 		msf = kmalloc(optlen, GFP_KERNEL);
 		if (!msf) {
 			err = -ENOBUFS;
 			break;
 		}
 		err = -EFAULT;
 		if (copy_from_user(msf, optval, optlen)) {
 			kfree(msf);
 			break;
 		}
 		 
 		if (msf->imsf_numsrc >= 0x3ffffffcU ||
 		    msf->imsf_numsrc > sysctl_igmp_max_msf) {
 			kfree(msf);
 			err = -ENOBUFS;
 			break;
 		}
 		if (IP_MSFILTER_SIZE(msf->imsf_numsrc) > optlen) {
 			kfree(msf);
 			err = -EINVAL;
 			break;
 		}
 		err = ip_mc_msfilter(sk, msf, 0);
 		kfree(msf);
 		break;
 	}
 	case IP_BLOCK_SOURCE:
 	case IP_UNBLOCK_SOURCE:
 	case IP_ADD_SOURCE_MEMBERSHIP:
 	case IP_DROP_SOURCE_MEMBERSHIP:
 	{
 		struct ip_mreq_source mreqs;
 		int omode, add;
 
 		if (optlen != sizeof(struct ip_mreq_source))
 			goto e_inval;
 		if (copy_from_user(&mreqs, optval, sizeof(mreqs))) {
 			err = -EFAULT;
 			break;
 		}
 		if (optname == IP_BLOCK_SOURCE) {
 			omode = MCAST_EXCLUDE;
 			add = 1;
 		} else if (optname == IP_UNBLOCK_SOURCE) {
 			omode = MCAST_EXCLUDE;
 			add = 0;
 		} else if (optname == IP_ADD_SOURCE_MEMBERSHIP) {
 			struct ip_mreqn mreq;
 
 			mreq.imr_multiaddr.s_addr = mreqs.imr_multiaddr;
 			mreq.imr_address.s_addr = mreqs.imr_interface;
 			mreq.imr_ifindex = 0;
 			err = ip_mc_join_group(sk, &mreq);
 			if (err && err != -EADDRINUSE)
 				break;
 			omode = MCAST_INCLUDE;
 			add = 1;
 		} else   {
 			omode = MCAST_INCLUDE;
 			add = 0;
 		}
 		err = ip_mc_source(add, omode, sk, &mreqs, 0);
 		break;
 	}
 	case MCAST_JOIN_GROUP:
 	case MCAST_LEAVE_GROUP:
 	{
 		struct group_req greq;
 		struct sockaddr_in *psin;
 		struct ip_mreqn mreq;
 
 		if (optlen < sizeof(struct group_req))
 			goto e_inval;
 		err = -EFAULT;
 		if (copy_from_user(&greq, optval, sizeof(greq)))
 			break;
 		psin = (struct sockaddr_in *)&greq.gr_group;
 		if (psin->sin_family != AF_INET)
 			goto e_inval;
 		memset(&mreq, 0, sizeof(mreq));
 		mreq.imr_multiaddr = psin->sin_addr;
 		mreq.imr_ifindex = greq.gr_interface;
 
 		if (optname == MCAST_JOIN_GROUP)
 			err = ip_mc_join_group(sk, &mreq);
 		else
 			err = ip_mc_leave_group(sk, &mreq);
 		break;
 	}
 	case MCAST_JOIN_SOURCE_GROUP:
 	case MCAST_LEAVE_SOURCE_GROUP:
 	case MCAST_BLOCK_SOURCE:
 	case MCAST_UNBLOCK_SOURCE:
 	{
 		struct group_source_req greqs;
 		struct ip_mreq_source mreqs;
 		struct sockaddr_in *psin;
 		int omode, add;
 
 		if (optlen != sizeof(struct group_source_req))
 			goto e_inval;
 		if (copy_from_user(&greqs, optval, sizeof(greqs))) {
 			err = -EFAULT;
 			break;
 		}
 		if (greqs.gsr_group.ss_family != AF_INET ||
 		    greqs.gsr_source.ss_family != AF_INET) {
 			err = -EADDRNOTAVAIL;
 			break;
 		}
 		psin = (struct sockaddr_in *)&greqs.gsr_group;
 		mreqs.imr_multiaddr = psin->sin_addr.s_addr;
 		psin = (struct sockaddr_in *)&greqs.gsr_source;
 		mreqs.imr_sourceaddr = psin->sin_addr.s_addr;
 		mreqs.imr_interface = 0;  
 
 		if (optname == MCAST_BLOCK_SOURCE) {
 			omode = MCAST_EXCLUDE;
 			add = 1;
 		} else if (optname == MCAST_UNBLOCK_SOURCE) {
 			omode = MCAST_EXCLUDE;
 			add = 0;
 		} else if (optname == MCAST_JOIN_SOURCE_GROUP) {
 			struct ip_mreqn mreq;
 
 			psin = (struct sockaddr_in *)&greqs.gsr_group;
 			mreq.imr_multiaddr = psin->sin_addr;
 			mreq.imr_address.s_addr = 0;
 			mreq.imr_ifindex = greqs.gsr_interface;
 			err = ip_mc_join_group(sk, &mreq);
 			if (err && err != -EADDRINUSE)
 				break;
 			greqs.gsr_interface = mreq.imr_ifindex;
 			omode = MCAST_INCLUDE;
 			add = 1;
 		} else   {
 			omode = MCAST_INCLUDE;
 			add = 0;
 		}
 		err = ip_mc_source(add, omode, sk, &mreqs,
 				   greqs.gsr_interface);
 		break;
 	}
 	case MCAST_MSFILTER:
 	{
 		struct sockaddr_in *psin;
 		struct ip_msfilter *msf = NULL;
 		struct group_filter *gsf = NULL;
 		int msize, i, ifindex;
 
 		if (optlen < GROUP_FILTER_SIZE(0))
 			goto e_inval;
 		if (optlen > sysctl_optmem_max) {
 			err = -ENOBUFS;
 			break;
 		}
 		gsf = kmalloc(optlen, GFP_KERNEL);
 		if (!gsf) {
 			err = -ENOBUFS;
 			break;
 		}
 		err = -EFAULT;
 		if (copy_from_user(gsf, optval, optlen))
 			goto mc_msf_out;
 
 		 
 		if (gsf->gf_numsrc >= 0x1ffffff ||
 		    gsf->gf_numsrc > sysctl_igmp_max_msf) {
 			err = -ENOBUFS;
 			goto mc_msf_out;
 		}
 		if (GROUP_FILTER_SIZE(gsf->gf_numsrc) > optlen) {
 			err = -EINVAL;
 			goto mc_msf_out;
 		}
 		msize = IP_MSFILTER_SIZE(gsf->gf_numsrc);
 		msf = kmalloc(msize, GFP_KERNEL);
 		if (!msf) {
 			err = -ENOBUFS;
 			goto mc_msf_out;
 		}
 		ifindex = gsf->gf_interface;
 		psin = (struct sockaddr_in *)&gsf->gf_group;
 		if (psin->sin_family != AF_INET) {
 			err = -EADDRNOTAVAIL;
 			goto mc_msf_out;
 		}
 		msf->imsf_multiaddr = psin->sin_addr.s_addr;
 		msf->imsf_interface = 0;
 		msf->imsf_fmode = gsf->gf_fmode;
 		msf->imsf_numsrc = gsf->gf_numsrc;
 		err = -EADDRNOTAVAIL;
 		for (i = 0; i < gsf->gf_numsrc; ++i) {
 			psin = (struct sockaddr_in *)&gsf->gf_slist[i];
 
 			if (psin->sin_family != AF_INET)
 				goto mc_msf_out;
 			msf->imsf_slist[i] = psin->sin_addr.s_addr;
 		}
 		kfree(gsf);
 		gsf = NULL;
 
 		err = ip_mc_msfilter(sk, msf, ifindex);
 mc_msf_out:
 		kfree(msf);
 		kfree(gsf);
 		break;
 	}
 	case IP_MULTICAST_ALL:
 		if (optlen < 1)
 			goto e_inval;
 		if (val != 0 && val != 1)
 			goto e_inval;
 		inet->mc_all = val;
 		break;
 	case IP_ROUTER_ALERT:
 		err = ip_ra_control(sk, val ? 1 : 0, NULL);
 		break;
 
 	case IP_FREEBIND:
 		if (optlen < 1)
 			goto e_inval;
 		inet->freebind = !!val;
 		break;
 
 	case IP_IPSEC_POLICY:
 	case IP_XFRM_POLICY:
 		err = -EPERM;
 		if (!capable(CAP_NET_ADMIN))
 			break;
 		err = xfrm_user_policy(sk, optname, optval, optlen);
 		break;
 
 	case IP_TRANSPARENT:
 		if (!capable(CAP_NET_ADMIN)) {
 			err = -EPERM;
 			break;
 		}
 		if (optlen < 1)
 			goto e_inval;
 		inet->transparent = !!val;
 		break;
 
 	case IP_MINTTL:
 		if (optlen < 1)
 			goto e_inval;
 		if (val < 0 || val > 255)
 			goto e_inval;
 		inet->min_ttl = val;
 		break;
 
 	default:
 		err = -ENOPROTOOPT;
 		break;
 	}
 	release_sock(sk);
 	return err;
 
 e_inval:
 	release_sock(sk);
 	return -EINVAL;
 }