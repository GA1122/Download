static int prb_queue_frozen(struct tpacket_kbdq_core *pkc)
{
	return pkc->reset_pending_on_curr_blk;
}