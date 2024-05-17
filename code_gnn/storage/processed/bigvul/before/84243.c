static inline void __queue_me(struct futex_q *q, struct futex_hash_bucket *hb)
{
	int prio;

	 
	prio = min(current->normal_prio, MAX_RT_PRIO);

	plist_node_init(&q->list, prio);
	plist_add(&q->list, &hb->chain);
	q->task = current;
}
