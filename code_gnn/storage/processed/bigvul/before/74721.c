void __attribute__ ((weak)) WatchdogHandler(struct pt_regs *regs)
{
	 
	mtspr(SPRN_TCR, mfspr(SPRN_TCR)&(~TCR_WIE));
	return;
}
