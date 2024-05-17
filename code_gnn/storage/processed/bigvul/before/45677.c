static void cryptd_queue_worker(struct work_struct *work)
{
	struct cryptd_cpu_queue *cpu_queue;
	struct crypto_async_request *req, *backlog;

	cpu_queue = container_of(work, struct cryptd_cpu_queue, work);
	 
	local_bh_disable();
	preempt_disable();
	backlog = crypto_get_backlog(&cpu_queue->queue);
	req = crypto_dequeue_request(&cpu_queue->queue);
	preempt_enable();
	local_bh_enable();

	if (!req)
		return;

	if (backlog)
		backlog->complete(backlog, -EINPROGRESS);
	req->complete(req, 0);

	if (cpu_queue->queue.qlen)
		queue_work(kcrypto_wq, &cpu_queue->work);
}