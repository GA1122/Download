int board_late_init(void)
{
#if defined(CONFIG_KM_COGE5UN)
	u8 dip_switch = kw_gpio_get_value(KM_FLASH_ERASE_ENABLE);

	 
	if (dip_switch != 0) {
		 
		puts("DIP:   Enabled\n");
		env_set("actual_bank", "0");
	}
#endif

#if defined(CONFIG_KM_FPGA_CONFIG)
	wait_for_fpga_config();
	fpga_reset();
	toggle_eeprom_spi_bus();
#endif
	return 0;
}
