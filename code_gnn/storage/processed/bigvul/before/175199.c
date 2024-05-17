SYSCALL_DEFINE2(setregid, gid_t, rgid, gid_t, egid)
{
 const struct cred *old;
 struct cred *new;
 int retval;

 new = prepare_creds();
 if (!new)
 return -ENOMEM;
	old = current_cred();

	retval = -EPERM;
 if (rgid != (gid_t) -1) {
 if (old->gid == rgid ||
		    old->egid == rgid ||
		    nsown_capable(CAP_SETGID))
 new->gid = rgid;
 else
 goto error;
 }
 if (egid != (gid_t) -1) {
 if (old->gid == egid ||
		    old->egid == egid ||
		    old->sgid == egid ||
		    nsown_capable(CAP_SETGID))
 new->egid = egid;
 else
 goto error;
 }

 if (rgid != (gid_t) -1 ||
 (egid != (gid_t) -1 && egid != old->gid))
 new->sgid = new->egid;
 new->fsgid = new->egid;

 return commit_creds(new);

error:
	abort_creds(new);
 return retval;
}
