int misc_init_r(void)
{
	ivm_read_eeprom(ivm_content, CONFIG_SYS_IVM_EEPROM_MAX_LEN,
			CONFIG_PIGGY_MAC_ADDRESS_OFFSET);

	initialize_unit_leds();
	set_km_env();
	set_bootcount_addr();
	return 0;
}
