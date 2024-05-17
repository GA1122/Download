validate_config(void)
{
#ifdef _WITH_VRRP_
	kernel_netlink_read_interfaces();
#endif

#ifdef _WITH_LVS_
	 
#ifndef _DEBUG_
	prog_type = PROG_TYPE_CHECKER;
#endif
	check_validate_config();
#endif
#ifdef _WITH_VRRP_
	 
#ifndef _DEBUG_
	prog_type = PROG_TYPE_VRRP;
#endif
	vrrp_validate_config();
#endif
#ifdef _WITH_BFD_
	 
#ifndef _DEBUG_
	prog_type = PROG_TYPE_BFD;
#endif
	bfd_validate_config();
#endif
}
