static long sctp_get_port_local(struct sock *sk, union sctp_addr *addr)
{
	struct sctp_bind_hashbucket *head;  
	struct sctp_bind_bucket *pp;  
	unsigned short snum;
	int ret;

	snum = ntohs(addr->v4.sin_port);

	SCTP_DEBUG_PRINTK("sctp_get_port() begins, snum=%d\n", snum);
	sctp_local_bh_disable();

	if (snum == 0) {
		 
		int low = sysctl_local_port_range[0];
		int high = sysctl_local_port_range[1];
		int remaining = (high - low) + 1;
		int rover;
		int index;

		sctp_spin_lock(&sctp_port_alloc_lock);
		rover = sctp_port_rover;
		do {
			rover++;
			if ((rover < low) || (rover > high))
				rover = low;
			index = sctp_phashfn(rover);
			head = &sctp_port_hashtable[index];
			sctp_spin_lock(&head->lock);
			for (pp = head->chain; pp; pp = pp->next)
				if (pp->port == rover)
					goto next;
			break;
		next:
			sctp_spin_unlock(&head->lock);
		} while (--remaining > 0);
		sctp_port_rover = rover;
		sctp_spin_unlock(&sctp_port_alloc_lock);

		 
		ret = 1;
		if (remaining <= 0)
			goto fail;

		 
		snum = rover;
	} else {
		 
		head = &sctp_port_hashtable[sctp_phashfn(snum)];
		sctp_spin_lock(&head->lock);
		for (pp = head->chain; pp; pp = pp->next) {
			if (pp->port == snum)
				goto pp_found;
		}
	}
	pp = NULL;
	goto pp_not_found;
pp_found:
	if (!hlist_empty(&pp->owner)) {
		 
		int reuse = sk->sk_reuse;
		struct sock *sk2;
		struct hlist_node *node;

		SCTP_DEBUG_PRINTK("sctp_get_port() found a possible match\n");
		if (pp->fastreuse && sk->sk_reuse)
			goto success;

		 
		sk_for_each_bound(sk2, node, &pp->owner) {
			struct sctp_endpoint *ep2;
			ep2 = sctp_sk(sk2)->ep;

			if (reuse && sk2->sk_reuse)
				continue;

			if (sctp_bind_addr_match(&ep2->base.bind_addr, addr,
						 sctp_sk(sk))) {
				ret = (long)sk2;
				goto fail_unlock;
			}
		}
		SCTP_DEBUG_PRINTK("sctp_get_port(): Found a match\n");
	}
pp_not_found:
	 
	ret = 1;
	if (!pp && !(pp = sctp_bucket_create(head, snum)))
		goto fail_unlock;

	 
	if (hlist_empty(&pp->owner))
		pp->fastreuse = sk->sk_reuse ? 1 : 0;
	else if (pp->fastreuse && !sk->sk_reuse)
		pp->fastreuse = 0;

	 
success:
	inet_sk(sk)->num = snum;
	if (!sctp_sk(sk)->bind_hash) {
		sk_add_bind_node(sk, &pp->owner);
		sctp_sk(sk)->bind_hash = pp;
	}
	ret = 0;

fail_unlock:
	sctp_spin_unlock(&head->lock);

fail:
	sctp_local_bh_enable();
	return ret;
}
