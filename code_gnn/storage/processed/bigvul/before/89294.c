int toggle_eeprom_spi_bus(void)
{
	int ret = 0;

	if (!check_boco2()) {
		 
		return 0;
	}

	ret = boco_set_bits(SPI_REG, CFG_EEPROM);
	if (ret)
		return ret;

	return 0;
}
