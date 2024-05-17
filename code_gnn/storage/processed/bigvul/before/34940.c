static void __rpc_execute(struct rpc_task *task)
{
	struct rpc_wait_queue *queue;
	int task_is_async = RPC_IS_ASYNC(task);
	int status = 0;

	dprintk("RPC: %5u __rpc_execute flags=0x%x\n",
			task->tk_pid, task->tk_flags);

	BUG_ON(RPC_IS_QUEUED(task));

	for (;;) {

		 
		if (task->tk_callback) {
			void (*save_callback)(struct rpc_task *);

			 
			save_callback = task->tk_callback;
			task->tk_callback = NULL;
			save_callback(task);
		} else {
			 
			if (task->tk_action == NULL)
				break;
			task->tk_action(task);
		}

		 
		if (!RPC_IS_QUEUED(task))
			continue;
		 
		queue = task->tk_waitqueue;
		spin_lock_bh(&queue->lock);
		if (!RPC_IS_QUEUED(task)) {
			spin_unlock_bh(&queue->lock);
			continue;
		}
		rpc_clear_running(task);
		spin_unlock_bh(&queue->lock);
		if (task_is_async)
			return;

		 
		dprintk("RPC: %5u sync task going to sleep\n", task->tk_pid);
		status = out_of_line_wait_on_bit(&task->tk_runstate,
				RPC_TASK_QUEUED, rpc_wait_bit_killable,
				TASK_KILLABLE);
		if (status == -ERESTARTSYS) {
			 
			dprintk("RPC: %5u got signal\n", task->tk_pid);
			task->tk_flags |= RPC_TASK_KILLED;
			rpc_exit(task, -ERESTARTSYS);
		}
		rpc_set_running(task);
		dprintk("RPC: %5u sync task resuming\n", task->tk_pid);
	}

	dprintk("RPC: %5u return %d, status %d\n", task->tk_pid, status,
			task->tk_status);
	 
	rpc_release_task(task);
}
