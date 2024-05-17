static void xen_blk_drain_io(struct xen_blkif *blkif)
{
	atomic_set(&blkif->drain, 1);
	do {
		 
		if (atomic_read(&blkif->refcnt) <= 2)
			break;
		wait_for_completion_interruptible_timeout(
				&blkif->drain_complete, HZ);

		if (!atomic_read(&blkif->drain))
			break;
	} while (!kthread_should_stop());
	atomic_set(&blkif->drain, 0);
}
