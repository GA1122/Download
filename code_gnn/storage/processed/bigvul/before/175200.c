SYSCALL_DEFINE2(setreuid, uid_t, ruid, uid_t, euid)
{
 const struct cred *old;
 struct cred *new;
 int retval;

 new = prepare_creds();
 if (!new)
 return -ENOMEM;
	old = current_cred();

	retval = -EPERM;
 if (ruid != (uid_t) -1) {
 new->uid = ruid;
 if (old->uid != ruid &&
		    old->euid != ruid &&
 !nsown_capable(CAP_SETUID))
 goto error;
 }

 if (euid != (uid_t) -1) {
 new->euid = euid;
 if (old->uid != euid &&
		    old->euid != euid &&
		    old->suid != euid &&
 !nsown_capable(CAP_SETUID))
 goto error;
 }

 if (new->uid != old->uid) {
		retval = set_user(new);
 if (retval < 0)
 goto error;
 }
 if (ruid != (uid_t) -1 ||
 (euid != (uid_t) -1 && euid != old->uid))
 new->suid = new->euid;
 new->fsuid = new->euid;

	retval = security_task_fix_setuid(new, old, LSM_SETID_RE);
 if (retval < 0)
 goto error;

 return commit_creds(new);

error:
	abort_creds(new);
 return retval;
}
