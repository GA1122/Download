start_keepalived(void)
{
	bool have_child = false;

#ifdef _WITH_BFD_
	 
	open_bfd_pipes();
#endif

#ifdef _WITH_LVS_
	 
	if (running_checker()) {
		start_check_child();
		have_child = true;
	}
#endif
#ifdef _WITH_VRRP_
	 
	if (running_vrrp()) {
		start_vrrp_child();
		have_child = true;
	}
#endif
#ifdef _WITH_BFD_
	 
	if (running_bfd()) {
		start_bfd_child();
		have_child = true;
	}
#endif

	return have_child;
}
