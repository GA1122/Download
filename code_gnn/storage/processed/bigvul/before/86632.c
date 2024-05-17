static void blk_flush_restore_request(struct request *rq)
{
	 
	rq->bio = rq->biotail;

	 
	rq->cmd_flags &= ~REQ_FLUSH_SEQ;
	rq->end_io = rq->flush.saved_end_io;
}
