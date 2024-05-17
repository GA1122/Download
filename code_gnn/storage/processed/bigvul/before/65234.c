nlmsvc_testlock(struct svc_rqst *rqstp, struct nlm_file *file,
		struct nlm_host *host, struct nlm_lock *lock,
		struct nlm_lock *conflock, struct nlm_cookie *cookie)
{
	int			error;
	__be32			ret;

	dprintk("lockd: nlmsvc_testlock(%s/%ld, ty=%d, %Ld-%Ld)\n",
				file_inode(file->f_file)->i_sb->s_id,
				file_inode(file->f_file)->i_ino,
				lock->fl.fl_type,
				(long long)lock->fl.fl_start,
				(long long)lock->fl.fl_end);

	if (locks_in_grace(SVC_NET(rqstp))) {
		ret = nlm_lck_denied_grace_period;
		goto out;
	}

	error = vfs_test_lock(file->f_file, &lock->fl);
	if (error) {
		 
		if (error == FILE_LOCK_DEFERRED)
			WARN_ON_ONCE(1);

		ret = nlm_lck_denied_nolocks;
		goto out;
	}

	if (lock->fl.fl_type == F_UNLCK) {
		ret = nlm_granted;
		goto out;
	}

	dprintk("lockd: conflicting lock(ty=%d, %Ld-%Ld)\n",
		lock->fl.fl_type, (long long)lock->fl.fl_start,
		(long long)lock->fl.fl_end);
	conflock->caller = "somehost";	 
	conflock->len = strlen(conflock->caller);
	conflock->oh.len = 0;		 
	conflock->svid = lock->fl.fl_pid;
	conflock->fl.fl_type = lock->fl.fl_type;
	conflock->fl.fl_start = lock->fl.fl_start;
	conflock->fl.fl_end = lock->fl.fl_end;
	locks_release_private(&lock->fl);
	ret = nlm_lck_denied;
out:
	return ret;
}