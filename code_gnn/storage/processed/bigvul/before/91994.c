struct request *blk_peek_request(struct request_queue *q)
{
	struct request *rq;
	int ret;

	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	while ((rq = elv_next_request(q)) != NULL) {
		if (!(rq->rq_flags & RQF_STARTED)) {
			 
			if (rq->rq_flags & RQF_SORTED)
				elv_activate_rq(q, rq);

			 
			rq->rq_flags |= RQF_STARTED;
			trace_block_rq_issue(q, rq);
		}

		if (!q->boundary_rq || q->boundary_rq == rq) {
			q->end_sector = rq_end_sector(rq);
			q->boundary_rq = NULL;
		}

		if (rq->rq_flags & RQF_DONTPREP)
			break;

		if (q->dma_drain_size && blk_rq_bytes(rq)) {
			 
			rq->nr_phys_segments++;
		}

		if (!q->prep_rq_fn)
			break;

		ret = q->prep_rq_fn(q, rq);
		if (ret == BLKPREP_OK) {
			break;
		} else if (ret == BLKPREP_DEFER) {
			 
			if (q->dma_drain_size && blk_rq_bytes(rq) &&
			    !(rq->rq_flags & RQF_DONTPREP)) {
				 
				--rq->nr_phys_segments;
			}

			rq = NULL;
			break;
		} else if (ret == BLKPREP_KILL || ret == BLKPREP_INVALID) {
			rq->rq_flags |= RQF_QUIET;
			 
			blk_start_request(rq);
			__blk_end_request_all(rq, ret == BLKPREP_INVALID ?
					BLK_STS_TARGET : BLK_STS_IOERR);
		} else {
			printk(KERN_ERR "%s: bad return=%d\n", __func__, ret);
			break;
		}
	}

	return rq;
}
