static void blk_mq_map_swqueue(struct request_queue *q)
{
	unsigned int i;
	struct blk_mq_hw_ctx *hctx;
	struct blk_mq_ctx *ctx;
	struct blk_mq_tag_set *set = q->tag_set;

	queue_for_each_hw_ctx(q, hctx, i) {
		cpumask_clear(hctx->cpumask);
		hctx->nr_ctx = 0;
	}

	 
	queue_for_each_ctx(q, ctx, i) {
		 
		if (!cpu_online(i))
			continue;

		hctx = q->mq_ops->map_queue(q, i);
		cpumask_set_cpu(i, hctx->cpumask);
		cpumask_set_cpu(i, hctx->tags->cpumask);
		ctx->index_hw = hctx->nr_ctx;
		hctx->ctxs[hctx->nr_ctx++] = ctx;
	}

	queue_for_each_hw_ctx(q, hctx, i) {
		struct blk_mq_ctxmap *map = &hctx->ctx_map;

		 
		if (!hctx->nr_ctx) {
			if (set->tags[i]) {
				blk_mq_free_rq_map(set, set->tags[i], i);
				set->tags[i] = NULL;
			}
			hctx->tags = NULL;
			continue;
		}

		 
		if (!set->tags[i])
			set->tags[i] = blk_mq_init_rq_map(set, i);
		hctx->tags = set->tags[i];
		WARN_ON(!hctx->tags);

		 
		map->size = DIV_ROUND_UP(hctx->nr_ctx, map->bits_per_word);

		 
		hctx->next_cpu = cpumask_first(hctx->cpumask);
		hctx->next_cpu_batch = BLK_MQ_CPU_WORK_BATCH;
	}
}
