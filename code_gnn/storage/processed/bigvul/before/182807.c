 int trigger_fpga_config(void)
 {
 	int ret = 0, init_l;
 	 
 	u32 timeout = 10000;
 
 	 
  	toggle_fpga_eeprom_bus(false);
  
  	 
	qrio_gpio_direction_output(GPIO_A, CONF_SEL_L, 0);
// 	qrio_gpio_direction_output(QRIO_GPIO_A, CONF_SEL_L, 0);
  
  	 
	qrio_gpio_direction_output(GPIO_A, FPGA_PROG_L, 0);
// 	qrio_gpio_direction_output(QRIO_GPIO_A, FPGA_PROG_L, 0);
  
  	 
  	udelay(10);
  
  	 
  	do {
		init_l = qrio_get_gpio(GPIO_A, FPGA_INIT_L);
// 		init_l = qrio_get_gpio(QRIO_GPIO_A, FPGA_INIT_L);
  		if (timeout-- == 0) {
  			printf("FPGA_INIT timeout\n");
  			ret = -EFAULT;
 			break;
 		}
 		udelay(10);
  	} while (init_l);
  
  	 
	qrio_set_gpio(GPIO_A, FPGA_PROG_L, 1);
// 	qrio_set_gpio(QRIO_GPIO_A, FPGA_PROG_L, 1);
  
  	return ret;
  }