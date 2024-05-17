int board_early_init_f(void)
{
	ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	bool cpuwd_flag = false;

	 
	qrio_uprstreq(UPREQ_CORE_RST);

	 
	setbits_be32(&gur->ddrclkdr, 0x001f000f);

	 
	if ((gur->rstrqsr1 & (RSTRQSR1_WDT_RR | RSTRQSR1_SW_RR)) ==
	    RSTRQSR1_WDT_RR)
		cpuwd_flag = true;

	qrio_cpuwd_flag(cpuwd_flag);
	 
	setbits_be32(&gur->rstrqsr1, RSTRQSR1_WDT_RR | RSTRQSR1_SW_RR);

	 
	qrio_prstcfg(BFTIC4_RST, PRSTCFG_POWUP_UNIT_RST);
	 
	qrio_wdmask(BFTIC4_RST, true);

	 
	qrio_prstcfg(ZL30158_RST, PRSTCFG_POWUP_RST);
	 
	qrio_prst(ZL30158_RST, false, false);

	return 0;
}
