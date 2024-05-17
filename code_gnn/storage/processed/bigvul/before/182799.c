  int trigger_fpga_config(void)
  {
  	int ret = 0;
  
// 	skip = 0;
// #ifndef CONFIG_KM_FPGA_FORCE_CONFIG
  	 
  	skip = 0;
 	if (fpga_done()) {
 		printf("PCIe FPGA config: skipped\n");
  		skip = 1;
  		return 0;
  	}
// #endif  
  
  	if (check_boco2()) {
  		 
 
 		 
 		ret = boco_clear_bits(SPI_REG, CFG_EEPROM);
 		if (ret)
 			return ret;
 
 		 
 		ret = boco_clear_bits(SPI_REG, FPGA_PROG | FPGA_INIT_B);
 		if (ret)
 			return ret;
 
 		 
 		udelay(10);
 
 		 
 		ret = boco_set_bits(SPI_REG, FPGA_PROG);
 		if (ret)
 			return ret;
 
 		 
 		ret = boco_set_bits(SPI_REG, FPGA_INIT_B);
 		if (ret)
 			return ret;
 
 	} else {
 		 
 		kw_gpio_set_valid(KM_XLX_PROGRAM_B_PIN, 1);
 		kw_gpio_direction_output(KM_XLX_PROGRAM_B_PIN, 0);
 		 
 		udelay(10);
 		kw_gpio_direction_input(KM_XLX_PROGRAM_B_PIN);
 	}
 
 	return 0;
 }