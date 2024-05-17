int board_init(void)
{
	 
	gd->bd->bi_boot_params = mvebu_sdram_bar(0) + 0x100;

	 
	kw_gpio_set_valid(KM_FLASH_GPIO_PIN, 1);
	kw_gpio_direction_output(KM_FLASH_GPIO_PIN, 1);

#if defined(CONFIG_SYS_I2C_SOFT)
	 
	kw_gpio_set_valid(KM_KIRKWOOD_SDA_PIN, 1);
	kw_gpio_set_valid(KM_KIRKWOOD_SCL_PIN, 1);
#endif

#if defined(CONFIG_SYS_EEPROM_WREN)
	kw_gpio_set_valid(KM_KIRKWOOD_ENV_WP, 38);
	kw_gpio_direction_output(KM_KIRKWOOD_ENV_WP, 1);
#endif

#if defined(CONFIG_KM_FPGA_CONFIG)
	trigger_fpga_config();
#endif

	return 0;
}
