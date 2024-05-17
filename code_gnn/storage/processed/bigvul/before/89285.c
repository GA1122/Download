void qrio_set_leds(void)
{
	u8 ctrlh;
	void __iomem *qrio_base = (void *)CONFIG_SYS_QRIO_BASE;

	 
	ctrlh = in_8(qrio_base + CTRLH_OFF);
	ctrlh |= (CTRLH_WRL_BOOT | CTRLH_WRL_UNITRUN);
	out_8(qrio_base + CTRLH_OFF, ctrlh);
}
