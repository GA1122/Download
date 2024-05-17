int cap_task_prctl(int option, unsigned long arg2, unsigned long arg3,
		   unsigned long arg4, unsigned long arg5)
{
	struct cred *new;
	long error = 0;

	new = prepare_creds();
	if (!new)
		return -ENOMEM;

	switch (option) {
	case PR_CAPBSET_READ:
		error = -EINVAL;
		if (!cap_valid(arg2))
			goto error;
		error = !!cap_raised(new->cap_bset, arg2);
		goto no_change;

	case PR_CAPBSET_DROP:
		error = cap_prctl_drop(new, arg2);
		if (error < 0)
			goto error;
		goto changed;

	 
	case PR_SET_SECUREBITS:
		error = -EPERM;
		if ((((new->securebits & SECURE_ALL_LOCKS) >> 1)
		     & (new->securebits ^ arg2))			 
		    || ((new->securebits & SECURE_ALL_LOCKS & ~arg2))	 
		    || (arg2 & ~(SECURE_ALL_LOCKS | SECURE_ALL_BITS))	 
		    || (cap_capable(current_cred(),
				    current_cred()->user->user_ns, CAP_SETPCAP,
				    SECURITY_CAP_AUDIT) != 0)		 
			 
		    )
			 
			goto error;
		new->securebits = arg2;
		goto changed;

	case PR_GET_SECUREBITS:
		error = new->securebits;
		goto no_change;

	case PR_GET_KEEPCAPS:
		if (issecure(SECURE_KEEP_CAPS))
			error = 1;
		goto no_change;

	case PR_SET_KEEPCAPS:
		error = -EINVAL;
		if (arg2 > 1)  
			goto error;
		error = -EPERM;
		if (issecure(SECURE_KEEP_CAPS_LOCKED))
			goto error;
		if (arg2)
			new->securebits |= issecure_mask(SECURE_KEEP_CAPS);
		else
			new->securebits &= ~issecure_mask(SECURE_KEEP_CAPS);
		goto changed;

	default:
		 
		error = -ENOSYS;
		goto error;
	}

	 
changed:
	return commit_creds(new);

no_change:
error:
	abort_creds(new);
	return error;
}
