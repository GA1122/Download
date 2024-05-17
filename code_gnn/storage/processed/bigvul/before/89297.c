int board_early_init_f(void)
{
#if defined(CONFIG_SYS_I2C_SOFT)
	u32 tmp;

	 
	tmp = readl(MVEBU_GPIO0_BASE + 4);
	writel(tmp & (~KM_KIRKWOOD_SOFT_I2C_GPIOS) , MVEBU_GPIO0_BASE + 4);
#endif
	 
	mvebu_sdram_size_adjust(0);
	kirkwood_mpp_conf(kwmpp_config, NULL);
	return 0;
}
