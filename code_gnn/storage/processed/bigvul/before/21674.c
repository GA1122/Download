static void aio_queue_work(struct kioctx * ctx)
{
	unsigned long timeout;
	 
	smp_mb();
	if (waitqueue_active(&ctx->wait))
		timeout = 1;
	else
		timeout = HZ/10;
	queue_delayed_work(aio_wq, &ctx->wq, timeout);
}
