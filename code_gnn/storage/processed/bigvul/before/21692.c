static int kiocb_batch_refill(struct kioctx *ctx, struct kiocb_batch *batch)
{
	unsigned short allocated, to_alloc;
	long avail;
	bool called_fput = false;
	struct kiocb *req, *n;
	struct aio_ring *ring;

	to_alloc = min(batch->count, KIOCB_BATCH_SIZE);
	for (allocated = 0; allocated < to_alloc; allocated++) {
		req = __aio_get_req(ctx);
		if (!req)
			 
			break;
		list_add(&req->ki_batch, &batch->head);
	}

	if (allocated == 0)
		goto out;

retry:
	spin_lock_irq(&ctx->ctx_lock);
	ring = kmap_atomic(ctx->ring_info.ring_pages[0]);

	avail = aio_ring_avail(&ctx->ring_info, ring) - ctx->reqs_active;
	BUG_ON(avail < 0);
	if (avail == 0 && !called_fput) {
		 
		kunmap_atomic(ring);
		spin_unlock_irq(&ctx->ctx_lock);
		aio_fput_routine(NULL);
		called_fput = true;
		goto retry;
	}

	if (avail < allocated) {
		 
		list_for_each_entry_safe(req, n, &batch->head, ki_batch) {
			list_del(&req->ki_batch);
			kmem_cache_free(kiocb_cachep, req);
			if (--allocated <= avail)
				break;
		}
	}

	batch->count -= allocated;
	list_for_each_entry(req, &batch->head, ki_batch) {
		list_add(&req->ki_list, &ctx->active_reqs);
		ctx->reqs_active++;
	}

	kunmap_atomic(ring);
	spin_unlock_irq(&ctx->ctx_lock);

out:
	return allocated;
}
