SYSCALL_DEFINE1(setuid, uid_t, uid)
{
 const struct cred *old;
 struct cred *new;
 int retval;

 new = prepare_creds();
 if (!new)
 return -ENOMEM;
	old = current_cred();

	retval = -EPERM;
 if (nsown_capable(CAP_SETUID)) {
 new->suid = new->uid = uid;
 if (uid != old->uid) {
			retval = set_user(new);
 if (retval < 0)
 goto error;
 }
 } else if (uid != old->uid && uid != new->suid) {
 goto error;
 }

 new->fsuid = new->euid = uid;

	retval = security_task_fix_setuid(new, old, LSM_SETID_ID);
 if (retval < 0)
 goto error;

 return commit_creds(new);

error:
	abort_creds(new);
 return retval;
}
