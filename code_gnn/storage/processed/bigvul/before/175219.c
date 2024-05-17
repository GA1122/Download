int do_prlimit(struct task_struct *tsk, unsigned int resource,
 struct rlimit *new_rlim, struct rlimit *old_rlim)
{
 struct rlimit *rlim;
 int retval = 0;

 if (resource >= RLIM_NLIMITS)
 return -EINVAL;
 if (new_rlim) {
 if (new_rlim->rlim_cur > new_rlim->rlim_max)
 return -EINVAL;
 if (resource == RLIMIT_NOFILE &&
				new_rlim->rlim_max > sysctl_nr_open)
 return -EPERM;
 }

  
	read_lock(&tasklist_lock);
 if (!tsk->sighand) {
		retval = -ESRCH;
 goto out;
 }

	rlim = tsk->signal->rlim + resource;
	task_lock(tsk->group_leader);
 if (new_rlim) {
  
 if (new_rlim->rlim_max > rlim->rlim_max &&
 !capable(CAP_SYS_RESOURCE))
			retval = -EPERM;
 if (!retval)
			retval = security_task_setrlimit(tsk->group_leader,
					resource, new_rlim);
 if (resource == RLIMIT_CPU && new_rlim->rlim_cur == 0) {
  
			new_rlim->rlim_cur = 1;
 }
 }
 if (!retval) {
 if (old_rlim)
 *old_rlim = *rlim;
 if (new_rlim)
 *rlim = *new_rlim;
 }
	task_unlock(tsk->group_leader);

  
 if (!retval && new_rlim && resource == RLIMIT_CPU &&
			 new_rlim->rlim_cur != RLIM_INFINITY)
		update_rlimit_cpu(tsk, new_rlim->rlim_cur);
out:
	read_unlock(&tasklist_lock);
 return retval;
}
