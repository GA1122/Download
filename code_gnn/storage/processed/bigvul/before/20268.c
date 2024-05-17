static inline int cap_inh_is_capped(void)
{

	 
	if (cap_capable(current_cred(), current_cred()->user->user_ns,
			CAP_SETPCAP, SECURITY_CAP_AUDIT) == 0)
		return 0;
	return 1;
}
