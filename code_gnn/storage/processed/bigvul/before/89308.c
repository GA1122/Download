int mvebu_board_spi_claim_bus(struct udevice *dev)
{
	spi_mpp_backup[3] = 0;

	 
	kirkwood_mpp_conf(spi_mpp_config, spi_mpp_backup);

	kw_gpio_set_value(KM_FLASH_GPIO_PIN, 0);

	return 0;
}
