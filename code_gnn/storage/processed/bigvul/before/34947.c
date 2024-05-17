static struct rpc_task * __rpc_wake_up_next_priority(struct rpc_wait_queue *queue)
{
	struct list_head *q;
	struct rpc_task *task;

	 
	q = &queue->tasks[queue->priority];
	if (!list_empty(q)) {
		task = list_entry(q->next, struct rpc_task, u.tk_wait.list);
		if (queue->owner == task->tk_owner) {
			if (--queue->nr)
				goto out;
			list_move_tail(&task->u.tk_wait.list, q);
		}
		 
		if (--queue->count)
			goto new_owner;
	}

	 
	do {
		if (q == &queue->tasks[0])
			q = &queue->tasks[queue->maxpriority];
		else
			q = q - 1;
		if (!list_empty(q)) {
			task = list_entry(q->next, struct rpc_task, u.tk_wait.list);
			goto new_queue;
		}
	} while (q != &queue->tasks[queue->priority]);

	rpc_reset_waitqueue_priority(queue);
	return NULL;

new_queue:
	rpc_set_waitqueue_priority(queue, (unsigned int)(q - &queue->tasks[0]));
new_owner:
	rpc_set_waitqueue_owner(queue, task->tk_owner);
out:
	rpc_wake_up_task_queue_locked(queue, task);
	return task;
}