static int check_restart(struct sem_array *sma, struct sem_queue *q)
{
	struct sem *curr;
	struct sem_queue *h;

	 
	if (q->alter == 0)
		return 0;

	 
	if (sma->complex_count)
		return 1;

	 
	if (q->nsops > 1)
		return 1;

	curr = sma->sem_base + q->sops[0].sem_num;

	 
	if (list_empty(&curr->sem_pending))
		return 0;

	 
	if (curr->semval) {
		 
		BUG_ON(q->sops[0].sem_op >= 0);
		return 0;
	}
	 
	h = list_first_entry(&curr->sem_pending, struct sem_queue, list);
	BUG_ON(h->nsops != 1);
	BUG_ON(h->sops[0].sem_num != q->sops[0].sem_num);

	 
	if (h->sops[0].sem_op == 0)
		return 1;

	 
	return 0;
}
