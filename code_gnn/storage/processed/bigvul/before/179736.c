 static int inet_create(struct net *net, struct socket *sock, int protocol,
 		       int kern)
 {
 	struct sock *sk;
 	struct inet_protosw *answer;
 	struct inet_sock *inet;
 	struct proto *answer_prot;
 	unsigned char answer_flags;
  	int try_loading_module = 0;
  	int err;
  
// 	if (protocol < 0 || protocol >= IPPROTO_MAX)
// 		return -EINVAL;
// 
  	sock->state = SS_UNCONNECTED;
  
  	 
 lookup_protocol:
 	err = -ESOCKTNOSUPPORT;
 	rcu_read_lock();
 	list_for_each_entry_rcu(answer, &inetsw[sock->type], list) {
 
 		err = 0;
 		 
 		if (protocol == answer->protocol) {
 			if (protocol != IPPROTO_IP)
 				break;
 		} else {
 			 
 			if (IPPROTO_IP == protocol) {
 				protocol = answer->protocol;
 				break;
 			}
 			if (IPPROTO_IP == answer->protocol)
 				break;
 		}
 		err = -EPROTONOSUPPORT;
 	}
 
 	if (unlikely(err)) {
 		if (try_loading_module < 2) {
 			rcu_read_unlock();
 			 
 			if (++try_loading_module == 1)
 				request_module("net-pf-%d-proto-%d-type-%d",
 					       PF_INET, protocol, sock->type);
 			 
 			else
 				request_module("net-pf-%d-proto-%d",
 					       PF_INET, protocol);
 			goto lookup_protocol;
 		} else
 			goto out_rcu_unlock;
 	}
 
 	err = -EPERM;
 	if (sock->type == SOCK_RAW && !kern &&
 	    !ns_capable(net->user_ns, CAP_NET_RAW))
 		goto out_rcu_unlock;
 
 	sock->ops = answer->ops;
 	answer_prot = answer->prot;
 	answer_flags = answer->flags;
 	rcu_read_unlock();
 
 	WARN_ON(!answer_prot->slab);
 
 	err = -ENOBUFS;
 	sk = sk_alloc(net, PF_INET, GFP_KERNEL, answer_prot, kern);
 	if (!sk)
 		goto out;
 
 	err = 0;
 	if (INET_PROTOSW_REUSE & answer_flags)
 		sk->sk_reuse = SK_CAN_REUSE;
 
 	inet = inet_sk(sk);
 	inet->is_icsk = (INET_PROTOSW_ICSK & answer_flags) != 0;
 
 	inet->nodefrag = 0;
 
 	if (SOCK_RAW == sock->type) {
 		inet->inet_num = protocol;
 		if (IPPROTO_RAW == protocol)
 			inet->hdrincl = 1;
 	}
 
 	if (net->ipv4.sysctl_ip_no_pmtu_disc)
 		inet->pmtudisc = IP_PMTUDISC_DONT;
 	else
 		inet->pmtudisc = IP_PMTUDISC_WANT;
 
 	inet->inet_id = 0;
 
 	sock_init_data(sock, sk);
 
 	sk->sk_destruct	   = inet_sock_destruct;
 	sk->sk_protocol	   = protocol;
 	sk->sk_backlog_rcv = sk->sk_prot->backlog_rcv;
 
 	inet->uc_ttl	= -1;
 	inet->mc_loop	= 1;
 	inet->mc_ttl	= 1;
 	inet->mc_all	= 1;
 	inet->mc_index	= 0;
 	inet->mc_list	= NULL;
 	inet->rcv_tos	= 0;
 
 	sk_refcnt_debug_inc(sk);
 
 	if (inet->inet_num) {
 		 
 		inet->inet_sport = htons(inet->inet_num);
 		 
 		sk->sk_prot->hash(sk);
 	}
 
 	if (sk->sk_prot->init) {
 		err = sk->sk_prot->init(sk);
 		if (err)
 			sk_common_release(sk);
 	}
 out:
 	return err;
 out_rcu_unlock:
 	rcu_read_unlock();
 	goto out;
 }