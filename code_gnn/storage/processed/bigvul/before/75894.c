global_init_keywords(void)
{
	 
	init_global_keywords(true);

#ifdef _WITH_VRRP_
	init_vrrp_keywords(false);
#endif
#ifdef _WITH_LVS_
	init_check_keywords(false);
#endif
#ifdef _WITH_BFD_
	init_bfd_keywords(false);
#endif

	return keywords;
}