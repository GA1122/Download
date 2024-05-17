propagate_signal(__attribute__((unused)) void *v, int sig)
{
	if (sig == SIGHUP) {
		if (!reload_config())
			return;
	}

	 
#ifdef _WITH_VRRP_
	if (vrrp_child > 0)
		kill(vrrp_child, sig);
	else if (sig == SIGHUP && running_vrrp())
		start_vrrp_child();
#endif
#ifdef _WITH_LVS_
	if (sig == SIGHUP) {
		if (checkers_child > 0)
			kill(checkers_child, sig);
		else if (running_checker())
			start_check_child();
	}
#endif
#ifdef _WITH_BFD_
	if (sig == SIGHUP) {
		if (bfd_child > 0)
			kill(bfd_child, sig);
		else if (running_bfd())
			start_bfd_child();
	}
#endif
}