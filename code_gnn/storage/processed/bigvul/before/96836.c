void fuse_wait_aborted(struct fuse_conn *fc)
{
	 
	smp_mb();
	wait_event(fc->blocked_waitq, atomic_read(&fc->num_waiting) == 0);
}
