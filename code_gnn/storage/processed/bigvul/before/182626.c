 static void flush_end_io(struct request *flush_rq, int error)
 {
 	struct request_queue *q = flush_rq->q;
 	struct list_head *running;
 	bool queued = false;
 	struct request *rq, *n;
 	unsigned long flags = 0;
  	struct blk_flush_queue *fq = blk_get_flush_queue(q, flush_rq->mq_ctx);
  
  	if (q->mq_ops) {
// 		struct blk_mq_hw_ctx *hctx;
// 
// 		 
  		spin_lock_irqsave(&fq->mq_flush_lock, flags);
// 		hctx = q->mq_ops->map_queue(q, flush_rq->mq_ctx->cpu);
// 		blk_mq_tag_set_rq(hctx, flush_rq->tag, fq->orig_rq);
  		flush_rq->tag = -1;
  	}
  
 	running = &fq->flush_queue[fq->flush_running_idx];
 	BUG_ON(fq->flush_pending_idx == fq->flush_running_idx);
 
 	 
 	fq->flush_running_idx ^= 1;
 
 	if (!q->mq_ops)
 		elv_completed_request(q, flush_rq);
 
 	 
 	list_for_each_entry_safe(rq, n, running, flush.list) {
 		unsigned int seq = blk_flush_cur_seq(rq);
 
 		BUG_ON(seq != REQ_FSEQ_PREFLUSH && seq != REQ_FSEQ_POSTFLUSH);
 		queued |= blk_flush_complete_seq(rq, fq, seq, error);
 	}
 
 	 
 	if (queued || fq->flush_queue_delayed) {
 		WARN_ON(q->mq_ops);
 		blk_run_queue_async(q);
 	}
 	fq->flush_queue_delayed = 0;
 	if (q->mq_ops)
 		spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
 }