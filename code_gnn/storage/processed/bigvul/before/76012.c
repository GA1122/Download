vrrp_init_keywords(void)
{
	 
	init_global_keywords(reload);

	init_vrrp_keywords(true);
#ifdef _WITH_LVS_
	init_check_keywords(false);
#endif
#ifdef _WITH_BFD_
	init_bfd_keywords(true);
#endif

	return keywords;
}