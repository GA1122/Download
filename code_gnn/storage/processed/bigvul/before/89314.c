void reset_phy(void)
{
#if defined(CONFIG_KM_MVEXTSW_ADDR)
	char *name = "egiga0";

	if (miiphy_set_current_dev(name))
		return;

	mv88e_sw_program(name, CONFIG_KM_MVEXTSW_ADDR, extsw_conf,
		ARRAY_SIZE(extsw_conf));
	mv88e_sw_reset(name, CONFIG_KM_MVEXTSW_ADDR);
#endif
}
