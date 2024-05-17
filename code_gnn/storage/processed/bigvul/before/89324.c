int misc_init_f(void)
{
	 
	i2c_deblock_gpio_cfg();

	 
	qrio_prstcfg(ETH_FRONT_PHY_RST, PRSTCFG_POWUP_UNIT_CORE_RST);
	qrio_prst(ETH_FRONT_PHY_RST, false, false);

	 
	qrio_prstcfg(ZL30343_RST, PRSTCFG_POWUP_RST);
	 
	qrio_wdmask(ZL30343_RST, true);

	 
	qrio_prstcfg(QSFP1_RST, PRSTCFG_POWUP_UNIT_RST);
	qrio_prstcfg(QSFP2_RST, PRSTCFG_POWUP_UNIT_RST);

	 
	qrio_wdmask(QSFP1_RST, true);
	qrio_wdmask(QSFP2_RST, true);

	return 0;
}
