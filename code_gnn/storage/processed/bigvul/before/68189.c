static struct sock *__llc_lookup_listener(struct llc_sap *sap,
					  struct llc_addr *laddr)
{
	struct sock *rc;
	struct hlist_nulls_node *node;
	int slot = llc_sk_laddr_hashfn(sap, laddr);
	struct hlist_nulls_head *laddr_hb = &sap->sk_laddr_hash[slot];

	rcu_read_lock();
again:
	sk_nulls_for_each_rcu(rc, node, laddr_hb) {
		if (llc_listener_match(sap, laddr, rc)) {
			 
			if (unlikely(!atomic_inc_not_zero(&rc->sk_refcnt)))
				goto again;
			if (unlikely(llc_sk(rc)->sap != sap ||
				     !llc_listener_match(sap, laddr, rc))) {
				sock_put(rc);
				continue;
			}
			goto found;
		}
	}
	rc = NULL;
	 
	if (unlikely(get_nulls_value(node) != slot))
		goto again;
found:
	rcu_read_unlock();
	return rc;
}
