SYSCALL_DEFINE3(setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid)
{
 const struct cred *old;
 struct cred *new;
 int retval;

 new = prepare_creds();
 if (!new)
 return -ENOMEM;
	old = current_cred();

	retval = -EPERM;
 if (!nsown_capable(CAP_SETGID)) {
 if (rgid != (gid_t) -1 && rgid != old->gid &&
		    rgid != old->egid  && rgid != old->sgid)
 goto error;
 if (egid != (gid_t) -1 && egid != old->gid &&
		    egid != old->egid  && egid != old->sgid)
 goto error;
 if (sgid != (gid_t) -1 && sgid != old->gid &&
		    sgid != old->egid  && sgid != old->sgid)
 goto error;
 }

 if (rgid != (gid_t) -1)
 new->gid = rgid;
 if (egid != (gid_t) -1)
 new->egid = egid;
 if (sgid != (gid_t) -1)
 new->sgid = sgid;
 new->fsgid = new->egid;

 return commit_creds(new);

error:
	abort_creds(new);
 return retval;
}
