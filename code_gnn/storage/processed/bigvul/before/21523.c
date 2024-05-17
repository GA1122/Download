SYSCALL_DEFINE2(getpriority, int, which, int, who)
{
	struct task_struct *g, *p;
	struct user_struct *user;
	const struct cred *cred = current_cred();
	long niceval, retval = -ESRCH;
	struct pid *pgrp;
	kuid_t uid;

	if (which > PRIO_USER || which < PRIO_PROCESS)
		return -EINVAL;

	rcu_read_lock();
	read_lock(&tasklist_lock);
	switch (which) {
		case PRIO_PROCESS:
			if (who)
				p = find_task_by_vpid(who);
			else
				p = current;
			if (p) {
				niceval = 20 - task_nice(p);
				if (niceval > retval)
					retval = niceval;
			}
			break;
		case PRIO_PGRP:
			if (who)
				pgrp = find_vpid(who);
			else
				pgrp = task_pgrp(current);
			do_each_pid_thread(pgrp, PIDTYPE_PGID, p) {
				niceval = 20 - task_nice(p);
				if (niceval > retval)
					retval = niceval;
			} while_each_pid_thread(pgrp, PIDTYPE_PGID, p);
			break;
		case PRIO_USER:
			uid = make_kuid(cred->user_ns, who);
			user = cred->user;
			if (!who)
				uid = cred->uid;
			else if (!uid_eq(uid, cred->uid) &&
				 !(user = find_user(uid)))
				goto out_unlock;	 

			do_each_thread(g, p) {
				if (uid_eq(task_uid(p), uid)) {
					niceval = 20 - task_nice(p);
					if (niceval > retval)
						retval = niceval;
				}
			} while_each_thread(g, p);
			if (!uid_eq(uid, cred->uid))
				free_uid(user);		 
			break;
	}
out_unlock:
	read_unlock(&tasklist_lock);
	rcu_read_unlock();

	return retval;
}
