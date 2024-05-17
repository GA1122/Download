static void flush_data_end_io(struct request *rq, int error)
{
	struct request_queue *q = rq->q;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, NULL);

	 
	if (blk_flush_complete_seq(rq, fq, REQ_FSEQ_DATA, error))
		blk_run_queue_async(q);
}
