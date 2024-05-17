static void peer_avl_rebalance(struct inet_peer __rcu **stack[],
			       struct inet_peer __rcu ***stackend,
			       struct inet_peer_base *base)
{
	struct inet_peer __rcu **nodep;
	struct inet_peer *node, *l, *r;
	int lh, rh;

	while (stackend > stack) {
		nodep = *--stackend;
		node = rcu_deref_locked(*nodep, base);
		l = rcu_deref_locked(node->avl_left, base);
		r = rcu_deref_locked(node->avl_right, base);
		lh = node_height(l);
		rh = node_height(r);
		if (lh > rh + 1) {  
			struct inet_peer *ll, *lr, *lrl, *lrr;
			int lrh;
			ll = rcu_deref_locked(l->avl_left, base);
			lr = rcu_deref_locked(l->avl_right, base);
			lrh = node_height(lr);
			if (lrh <= node_height(ll)) {	 
				RCU_INIT_POINTER(node->avl_left, lr);	 
				RCU_INIT_POINTER(node->avl_right, r);	 
				node->avl_height = lrh + 1;  
				RCU_INIT_POINTER(l->avl_left, ll);        
				RCU_INIT_POINTER(l->avl_right, node);	 
				l->avl_height = node->avl_height + 1;
				RCU_INIT_POINTER(*nodep, l);
			} else {  
				lrl = rcu_deref_locked(lr->avl_left, base); 
				lrr = rcu_deref_locked(lr->avl_right, base); 
				RCU_INIT_POINTER(node->avl_left, lrr);	 
				RCU_INIT_POINTER(node->avl_right, r);	 
				node->avl_height = rh + 1;  
				RCU_INIT_POINTER(l->avl_left, ll);	 
				RCU_INIT_POINTER(l->avl_right, lrl);	 
				l->avl_height = rh + 1;	 
				RCU_INIT_POINTER(lr->avl_left, l);	 
				RCU_INIT_POINTER(lr->avl_right, node);	 
				lr->avl_height = rh + 2;
				RCU_INIT_POINTER(*nodep, lr);
			}
		} else if (rh > lh + 1) {  
			struct inet_peer *rr, *rl, *rlr, *rll;
			int rlh;
			rr = rcu_deref_locked(r->avl_right, base);
			rl = rcu_deref_locked(r->avl_left, base);
			rlh = node_height(rl);
			if (rlh <= node_height(rr)) {	 
				RCU_INIT_POINTER(node->avl_right, rl);	 
				RCU_INIT_POINTER(node->avl_left, l);	 
				node->avl_height = rlh + 1;  
				RCU_INIT_POINTER(r->avl_right, rr);	 
				RCU_INIT_POINTER(r->avl_left, node);	 
				r->avl_height = node->avl_height + 1;
				RCU_INIT_POINTER(*nodep, r);
			} else {  
				rlr = rcu_deref_locked(rl->avl_right, base); 
				rll = rcu_deref_locked(rl->avl_left, base); 
				RCU_INIT_POINTER(node->avl_right, rll);	 
				RCU_INIT_POINTER(node->avl_left, l);	 
				node->avl_height = lh + 1;  
				RCU_INIT_POINTER(r->avl_right, rr);	 
				RCU_INIT_POINTER(r->avl_left, rlr);	 
				r->avl_height = lh + 1;	 
				RCU_INIT_POINTER(rl->avl_right, r);	 
				RCU_INIT_POINTER(rl->avl_left, node);	 
				rl->avl_height = lh + 2;
				RCU_INIT_POINTER(*nodep, rl);
			}
		} else {
			node->avl_height = (lh > rh ? lh : rh) + 1;
		}
	}
}
