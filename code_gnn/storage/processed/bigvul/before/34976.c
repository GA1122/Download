void rpc_sleep_on(struct rpc_wait_queue *q, struct rpc_task *task,
				rpc_action action)
{
	 
	BUG_ON(!RPC_IS_ACTIVATED(task));

	 
	spin_lock_bh(&q->lock);
	__rpc_sleep_on(q, task, action);
	spin_unlock_bh(&q->lock);
}
