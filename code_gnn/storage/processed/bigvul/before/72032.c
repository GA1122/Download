static void free_ioctx_reqs(struct percpu_ref *ref)
{
	struct kioctx *ctx = container_of(ref, struct kioctx, reqs);

	 
	if (ctx->rq_wait && atomic_dec_and_test(&ctx->rq_wait->count))
		complete(&ctx->rq_wait->comp);

	INIT_WORK(&ctx->free_work, free_ioctx);
	schedule_work(&ctx->free_work);
}