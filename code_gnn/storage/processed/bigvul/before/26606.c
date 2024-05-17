static int inet_peer_gc(struct inet_peer_base *base,
			struct inet_peer __rcu **stack[PEER_MAXDEPTH],
			struct inet_peer __rcu ***stackptr)
{
	struct inet_peer *p, *gchead = NULL;
	__u32 delta, ttl;
	int cnt = 0;

	if (base->total >= inet_peer_threshold)
		ttl = 0;  
	else
		ttl = inet_peer_maxttl
				- (inet_peer_maxttl - inet_peer_minttl) / HZ *
					base->total / inet_peer_threshold * HZ;
	stackptr--;  
	while (stackptr > stack) {
		stackptr--;
		p = rcu_deref_locked(**stackptr, base);
		if (atomic_read(&p->refcnt) == 0) {
			smp_rmb();
			delta = (__u32)jiffies - p->dtime;
			if (delta >= ttl &&
			    atomic_cmpxchg(&p->refcnt, 0, -1) == 0) {
				p->gc_next = gchead;
				gchead = p;
			}
		}
	}
	while ((p = gchead) != NULL) {
		gchead = p->gc_next;
		cnt++;
		unlink_from_pool(p, base, stack);
	}
 	return cnt;
 }