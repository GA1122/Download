int mvebu_board_spi_release_bus(struct udevice *dev)
{
	 
	kirkwood_mpp_conf(spi_mpp_backup, NULL);

	kw_gpio_set_value(KM_FLASH_GPIO_PIN, 1);

	return 0;
}
