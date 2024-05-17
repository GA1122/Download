static int nlmsvc_unlink_block(struct nlm_block *block)
{
	int status;
	dprintk("lockd: unlinking block %p...\n", block);

	 
	status = posix_unblock_lock(&block->b_call->a_args.lock.fl);
	nlmsvc_remove_block(block);
	return status;
}