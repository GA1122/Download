static int check_prlimit_permission(struct task_struct *task)
{
 const struct cred *cred = current_cred(), *tcred;

 if (current == task)
 return 0;

	tcred = __task_cred(task);
 if (cred->user->user_ns == tcred->user->user_ns &&
 (cred->uid == tcred->euid &&
	     cred->uid == tcred->suid &&
	     cred->uid == tcred->uid  &&
	     cred->gid == tcred->egid &&
	     cred->gid == tcred->sgid &&
	     cred->gid == tcred->gid))
 return 0;
 if (ns_capable(tcred->user->user_ns, CAP_SYS_RESOURCE))
 return 0;

 return -EPERM;
}
