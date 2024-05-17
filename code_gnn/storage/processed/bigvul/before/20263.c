int cap_capable(const struct cred *cred, struct user_namespace *targ_ns,
		int cap, int audit)
{
	for (;;) {
		 
		if (targ_ns != &init_user_ns && targ_ns->creator == cred->user)
			return 0;

		 
		if (targ_ns == cred->user->user_ns)
			return cap_raised(cred->cap_effective, cap) ? 0 : -EPERM;

		 
		if (targ_ns == &init_user_ns)
			return -EPERM;

		 
		targ_ns = targ_ns->creator->user_ns;
	}

	 
}
