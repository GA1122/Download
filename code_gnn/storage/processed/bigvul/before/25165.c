static void rt_del(unsigned hash, struct rtable *rt)
{
	struct rtable __rcu **rthp;
	struct rtable *aux;

	rthp = &rt_hash_table[hash].chain;
	spin_lock_bh(rt_hash_lock_addr(hash));
	ip_rt_put(rt);
	while ((aux = rcu_dereference_protected(*rthp,
			lockdep_is_held(rt_hash_lock_addr(hash)))) != NULL) {
		if (aux == rt || rt_is_expired(aux)) {
			*rthp = aux->dst.rt_next;
			rt_free(aux);
			continue;
		}
		rthp = &aux->dst.rt_next;
	}
	spin_unlock_bh(rt_hash_lock_addr(hash));
}
