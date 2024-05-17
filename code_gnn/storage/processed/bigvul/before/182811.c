 void board_init_f_init_reserve(ulong base)
 {
 	struct global_data *gd_ptr;
 
 	 
 
 	gd_ptr = (struct global_data *)base;
 	 
 	memset(gd_ptr, '\0', sizeof(*gd));
 	 
 #if !defined(CONFIG_ARM)
 	arch_setup_gd(gd_ptr);
 #endif
 
 	if (CONFIG_IS_ENABLED(SYS_REPORT_STACK_F_USAGE))
 		board_init_f_init_stack_protection_addr(base);
 
 	 
 	base += roundup(sizeof(struct global_data), 16);
 
 	 
 
  #if CONFIG_VAL(SYS_MALLOC_F_LEN)
  	 
  	gd->malloc_base = base;
	 
	base += CONFIG_VAL(SYS_MALLOC_F_LEN);
  #endif
  
  	if (CONFIG_IS_ENABLED(SYS_REPORT_STACK_F_USAGE))
 		board_init_f_init_stack_protection();
 }