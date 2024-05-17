static long sctp_get_port_local(struct sock *sk, union sctp_addr *addr)
{
	struct sctp_bind_hashbucket *head;  
	struct sctp_bind_bucket *pp;
	unsigned short snum;
	int ret;

	snum = ntohs(addr->v4.sin_port);

	pr_debug("%s: begins, snum:%d\n", __func__, snum);

	local_bh_disable();

	if (snum == 0) {
		 
		int low, high, remaining, index;
		unsigned int rover;
		struct net *net = sock_net(sk);

		inet_get_local_port_range(net, &low, &high);
		remaining = (high - low) + 1;
		rover = prandom_u32() % remaining + low;

		do {
			rover++;
			if ((rover < low) || (rover > high))
				rover = low;
			if (inet_is_local_reserved_port(net, rover))
				continue;
			index = sctp_phashfn(sock_net(sk), rover);
			head = &sctp_port_hashtable[index];
			spin_lock(&head->lock);
			sctp_for_each_hentry(pp, &head->chain)
				if ((pp->port == rover) &&
				    net_eq(sock_net(sk), pp->net))
					goto next;
			break;
		next:
			spin_unlock(&head->lock);
		} while (--remaining > 0);

		 
		ret = 1;
		if (remaining <= 0)
			goto fail;

		 
		snum = rover;
	} else {
		 
		head = &sctp_port_hashtable[sctp_phashfn(sock_net(sk), snum)];
		spin_lock(&head->lock);
		sctp_for_each_hentry(pp, &head->chain) {
			if ((pp->port == snum) && net_eq(pp->net, sock_net(sk)))
				goto pp_found;
		}
	}
	pp = NULL;
	goto pp_not_found;
pp_found:
	if (!hlist_empty(&pp->owner)) {
		 
		int reuse = sk->sk_reuse;
		struct sock *sk2;

		pr_debug("%s: found a possible match\n", __func__);

		if (pp->fastreuse && sk->sk_reuse &&
			sk->sk_state != SCTP_SS_LISTENING)
			goto success;

		 
		sk_for_each_bound(sk2, &pp->owner) {
			struct sctp_endpoint *ep2;
			ep2 = sctp_sk(sk2)->ep;

			if (sk == sk2 ||
			    (reuse && sk2->sk_reuse &&
			     sk2->sk_state != SCTP_SS_LISTENING))
				continue;

			if (sctp_bind_addr_conflict(&ep2->base.bind_addr, addr,
						 sctp_sk(sk2), sctp_sk(sk))) {
				ret = (long)sk2;
				goto fail_unlock;
			}
		}

		pr_debug("%s: found a match\n", __func__);
	}
pp_not_found:
	 
	ret = 1;
	if (!pp && !(pp = sctp_bucket_create(head, sock_net(sk), snum)))
		goto fail_unlock;

	 
	if (hlist_empty(&pp->owner)) {
		if (sk->sk_reuse && sk->sk_state != SCTP_SS_LISTENING)
			pp->fastreuse = 1;
		else
			pp->fastreuse = 0;
	} else if (pp->fastreuse &&
		(!sk->sk_reuse || sk->sk_state == SCTP_SS_LISTENING))
		pp->fastreuse = 0;

	 
success:
	if (!sctp_sk(sk)->bind_hash) {
		inet_sk(sk)->inet_num = snum;
		sk_add_bind_node(sk, &pp->owner);
		sctp_sk(sk)->bind_hash = pp;
	}
	ret = 0;

fail_unlock:
	spin_unlock(&head->lock);

fail:
	local_bh_enable();
	return ret;
}
