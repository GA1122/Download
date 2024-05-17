SYSCALL_DEFINE1(setfsgid, gid_t, gid)
{
 const struct cred *old;
 struct cred *new;
 gid_t old_fsgid;

 new = prepare_creds();
 if (!new)
 return current_fsgid();
	old = current_cred();
	old_fsgid = old->fsgid;

 if (gid == old->gid  || gid == old->egid  ||
	    gid == old->sgid || gid == old->fsgid ||
	    nsown_capable(CAP_SETGID)) {
 if (gid != old_fsgid) {
 new->fsgid = gid;
 goto change_okay;
 }
 }

	abort_creds(new);
 return old_fsgid;

change_okay:
	commit_creds(new);
 return old_fsgid;
}
