static bool check_same_owner(struct task_struct *p)
{
	const struct cred *cred = current_cred(), *pcred;
	bool match;

	rcu_read_lock();
	pcred = __task_cred(p);
	if (cred->user->user_ns == pcred->user->user_ns)
		match = (cred->euid == pcred->euid ||
			 cred->euid == pcred->uid);
	else
		match = false;
	rcu_read_unlock();
	return match;
}