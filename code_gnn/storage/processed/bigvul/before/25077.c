void inet_hashinfo_init(struct inet_hashinfo *h)
{
	int i;

	atomic_set(&h->bsockets, 0);
	for (i = 0; i < INET_LHTABLE_SIZE; i++) {
		spin_lock_init(&h->listening_hash[i].lock);
		INIT_HLIST_NULLS_HEAD(&h->listening_hash[i].head,
				      i + LISTENING_NULLS_BASE);
		}
}
