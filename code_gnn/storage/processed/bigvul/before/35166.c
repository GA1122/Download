static int check_kill_permission(int sig, struct siginfo *info,
				 struct task_struct *t)
{
	const struct cred *cred, *tcred;
	struct pid *sid;
	int error;

	if (!valid_signal(sig))
		return -EINVAL;

	if (!si_fromuser(info))
		return 0;

	error = audit_signal_info(sig, t);  
	if (error)
		return error;

	cred = current_cred();
	tcred = __task_cred(t);
	if (!same_thread_group(current, t) &&
	    (cred->euid ^ tcred->suid) &&
	    (cred->euid ^ tcred->uid) &&
	    (cred->uid  ^ tcred->suid) &&
	    (cred->uid  ^ tcred->uid) &&
	    !capable(CAP_KILL)) {
		switch (sig) {
		case SIGCONT:
			sid = task_session(t);
			 
			if (!sid || sid == task_session(current))
				break;
		default:
			return -EPERM;
		}
	}

	return security_task_kill(t, info, sig, 0);
}
