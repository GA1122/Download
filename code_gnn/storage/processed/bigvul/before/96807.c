static void fuse_drop_waiting(struct fuse_conn *fc)
{
	 
	if (atomic_dec_and_test(&fc->num_waiting) &&
	    !READ_ONCE(fc->connected)) {
		 
		wake_up_all(&fc->blocked_waitq);
	}
}
