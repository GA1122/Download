static void mcryptd_queue_worker(struct work_struct *work)
{
	struct mcryptd_cpu_queue *cpu_queue;
	struct crypto_async_request *req, *backlog;
	int i;

	 

	cpu_queue = container_of(work, struct mcryptd_cpu_queue, work);
	i = 0;
	while (i < MCRYPTD_BATCH || single_task_running()) {
		 
		local_bh_disable();
		preempt_disable();
		backlog = crypto_get_backlog(&cpu_queue->queue);
		req = crypto_dequeue_request(&cpu_queue->queue);
		preempt_enable();
		local_bh_enable();

		if (!req) {
			mcryptd_opportunistic_flush();
			return;
		}

		if (backlog)
			backlog->complete(backlog, -EINPROGRESS);
		req->complete(req, 0);
		if (!cpu_queue->queue.qlen)
			return;
		++i;
	}
	if (cpu_queue->queue.qlen)
		queue_work(kcrypto_wq, &cpu_queue->work);
}
