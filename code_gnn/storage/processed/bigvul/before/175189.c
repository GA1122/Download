SYSCALL_DEFINE1(setfsuid, uid_t, uid)
{
 const struct cred *old;
 struct cred *new;
 uid_t old_fsuid;

 new = prepare_creds();
 if (!new)
 return current_fsuid();
	old = current_cred();
	old_fsuid = old->fsuid;

 if (uid == old->uid  || uid == old->euid  ||
	    uid == old->suid || uid == old->fsuid ||
	    nsown_capable(CAP_SETUID)) {
 if (uid != old_fsuid) {
 new->fsuid = uid;
 if (security_task_fix_setuid(new, old, LSM_SETID_FS) == 0)
 goto change_okay;
 }
 }

	abort_creds(new);
 return old_fsuid;

change_okay:
	commit_creds(new);
 return old_fsuid;
}
