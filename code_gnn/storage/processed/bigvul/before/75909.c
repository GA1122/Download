stop_keepalived(void)
{
#ifndef _DEBUG_
	 
	thread_destroy_master(master);

#ifdef _WITH_VRRP_
	if (__test_bit(DAEMON_VRRP, &daemon_mode))
		pidfile_rm(vrrp_pidfile);
#endif

#ifdef _WITH_LVS_
	if (__test_bit(DAEMON_CHECKERS, &daemon_mode))
		pidfile_rm(checkers_pidfile);
#endif

#ifdef _WITH_BFD_
	if (__test_bit(DAEMON_BFD, &daemon_mode))
		pidfile_rm(bfd_pidfile);
#endif

	pidfile_rm(main_pidfile);
#endif
}