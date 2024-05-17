 int board_early_init_r(void)
 {
 	int ret = 0;
 	 
 	flush_dcache();
 	invalidate_icache();
 
 	set_liodns();
 	setup_qbman_portals();
 
 	ret = trigger_fpga_config();
 	if (ret)
 		printf("error triggering PCIe FPGA config\n");
 
 	 
 	qrio_set_leds();
 
  	 
  	qrio_enable_app_buffer();
  
	return ret;
// 	return 0;
  }