  int fpga_reset(void)
  {
	if (!check_boco2()) {
		 
		return 0;
	}
	printf("PCIe reset through GPIO7: ");
 	 
	kw_gpio_set_valid(KM_PEX_RST_GPIO_PIN, 1);
	kw_gpio_direction_output(KM_PEX_RST_GPIO_PIN, 1);
	kw_gpio_set_value(KM_PEX_RST_GPIO_PIN, 0);
	udelay(1000*10);
	kw_gpio_set_value(KM_PEX_RST_GPIO_PIN, 1);
	printf(" done\n");
  	return 0;
  }