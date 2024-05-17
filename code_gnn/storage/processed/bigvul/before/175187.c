SYSCALL_DEFINE1(setgid, gid_t, gid)
{
 const struct cred *old;
 struct cred *new;
 int retval;

 new = prepare_creds();
 if (!new)
 return -ENOMEM;
	old = current_cred();

	retval = -EPERM;
 if (nsown_capable(CAP_SETGID))
 new->gid = new->egid = new->sgid = new->fsgid = gid;
 else if (gid == old->gid || gid == old->sgid)
 new->egid = new->fsgid = gid;
 else
 goto error;

 return commit_creds(new);

error:
	abort_creds(new);
 return retval;
}
