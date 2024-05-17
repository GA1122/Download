static void unlink_from_pool(struct inet_peer *p, struct inet_peer_base *base,
			     struct inet_peer __rcu **stack[PEER_MAXDEPTH])
{
	struct inet_peer __rcu ***stackptr, ***delp;

	if (lookup(&p->daddr, stack, base) != p)
		BUG();
	delp = stackptr - 1;  
	if (p->avl_left == peer_avl_empty_rcu) {
		*delp[0] = p->avl_right;
		--stackptr;
	} else {
		 
		struct inet_peer *t;
		t = lookup_rightempty(p, base);
		BUG_ON(rcu_deref_locked(*stackptr[-1], base) != t);
		**--stackptr = t->avl_left;
		 
		RCU_INIT_POINTER(*delp[0], t);
		t->avl_left = p->avl_left;
		t->avl_right = p->avl_right;
		t->avl_height = p->avl_height;
		BUG_ON(delp[1] != &p->avl_left);
		delp[1] = &t->avl_left;  
	}
	peer_avl_rebalance(stack, stackptr, base);
	base->total--;
	call_rcu(&p->rcu, inetpeer_free_rcu);
}
