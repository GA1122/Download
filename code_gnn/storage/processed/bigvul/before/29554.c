static int update_queue(struct sem_array *sma, int semnum, struct list_head *pt)
{
	struct sem_queue *q;
	struct list_head *walk;
	struct list_head *pending_list;
	int semop_completed = 0;

	if (semnum == -1)
		pending_list = &sma->sem_pending;
	else
		pending_list = &sma->sem_base[semnum].sem_pending;

again:
	walk = pending_list->next;
	while (walk != pending_list) {
		int error, restart;

		q = container_of(walk, struct sem_queue, list);
		walk = walk->next;

		 
		if (semnum != -1 && sma->sem_base[semnum].semval == 0 &&
				q->alter)
			break;

		error = try_atomic_semop(sma, q->sops, q->nsops,
					 q->undo, q->pid);

		 
		if (error > 0)
			continue;

		unlink_queue(sma, q);

		if (error) {
			restart = 0;
		} else {
			semop_completed = 1;
			restart = check_restart(sma, q);
		}

		wake_up_sem_queue_prepare(pt, q, error);
		if (restart)
			goto again;
	}
	return semop_completed;
}
