int nlmclnt_proc(struct nlm_host *host, int cmd, struct file_lock *fl)
{
	struct nlm_rqst		*call;
	int			status;

	nlm_get_host(host);
	call = nlm_alloc_call(host);
	if (call == NULL)
		return -ENOMEM;

	nlmclnt_locks_init_private(fl, host);
	 
	nlmclnt_setlockargs(call, fl);

	if (IS_SETLK(cmd) || IS_SETLKW(cmd)) {
		if (fl->fl_type != F_UNLCK) {
			call->a_args.block = IS_SETLKW(cmd) ? 1 : 0;
			status = nlmclnt_lock(call, fl);
		} else
			status = nlmclnt_unlock(call, fl);
	} else if (IS_GETLK(cmd))
		status = nlmclnt_test(call, fl);
	else
		status = -EINVAL;
	fl->fl_ops->fl_release_private(fl);
	fl->fl_ops = NULL;

	dprintk("lockd: clnt proc returns %d\n", status);
	return status;
}