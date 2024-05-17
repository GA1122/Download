int misc_init_r(void)
{
	serdes_corenet_t *regs = (void *)CONFIG_SYS_FSL_CORENET_SERDES_ADDR;
	u32 expected[NUM_SRDS_BANKS] = {SRDS_PLLCR0_RFCK_SEL_100,
		SRDS_PLLCR0_RFCK_SEL_125};
	unsigned int i;

	 
	for (i = 0; i < NUM_SRDS_BANKS; i++) {
		u32 actual = in_be32(&regs->bank[i].pllcr0);
		actual &= SRDS_PLLCR0_RFCK_SEL_MASK;
		if (actual != expected[i]) {
			printf("Warning: SERDES bank %u expects reference \
			       clock %sMHz, but actual is %sMHz\n", i + 1,
			       serdes_clock_to_string(expected[i]),
			       serdes_clock_to_string(actual));
		}
	}

	ivm_read_eeprom(ivm_content, CONFIG_SYS_IVM_EEPROM_MAX_LEN,
			CONFIG_PIGGY_MAC_ADDRESS_OFFSET);
	return 0;
}
