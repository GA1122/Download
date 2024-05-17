nlmsvc_grant_blocked(struct nlm_block *block)
{
	struct nlm_file		*file = block->b_file;
	struct nlm_lock		*lock = &block->b_call->a_args.lock;
	int			error;
	loff_t			fl_start, fl_end;

	dprintk("lockd: grant blocked lock %p\n", block);

	kref_get(&block->b_count);

	 
	nlmsvc_unlink_block(block);

	 
	if (block->b_granted) {
		nlm_rebind_host(block->b_host);
		goto callback;
	}

	 
	 
	lock->fl.fl_flags |= FL_SLEEP;
	fl_start = lock->fl.fl_start;
	fl_end = lock->fl.fl_end;
	error = vfs_lock_file(file->f_file, F_SETLK, &lock->fl, NULL);
	lock->fl.fl_flags &= ~FL_SLEEP;
	lock->fl.fl_start = fl_start;
	lock->fl.fl_end = fl_end;

	switch (error) {
	case 0:
		break;
	case FILE_LOCK_DEFERRED:
		dprintk("lockd: lock still blocked error %d\n", error);
		nlmsvc_insert_block(block, NLM_NEVER);
		nlmsvc_release_block(block);
		return;
	default:
		printk(KERN_WARNING "lockd: unexpected error %d in %s!\n",
				-error, __func__);
		nlmsvc_insert_block(block, 10 * HZ);
		nlmsvc_release_block(block);
		return;
	}

callback:
	 
	dprintk("lockd: GRANTing blocked lock.\n");
	block->b_granted = 1;

	 
	nlmsvc_insert_block(block, NLM_NEVER);

	 
	error = nlm_async_call(block->b_call, NLMPROC_GRANTED_MSG,
				&nlmsvc_grant_ops);

	 
	if (error < 0)
		nlmsvc_insert_block(block, 10 * HZ);
}
