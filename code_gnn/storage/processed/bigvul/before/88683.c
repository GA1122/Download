static void dwc3_gadget_setup_nump(struct dwc3 *dwc)
{
	u32 ram2_depth;
	u32 mdwidth;
	u32 nump;
	u32 reg;

	ram2_depth = DWC3_GHWPARAMS7_RAM2_DEPTH(dwc->hwparams.hwparams7);
	mdwidth = DWC3_GHWPARAMS0_MDWIDTH(dwc->hwparams.hwparams0);

	nump = ((ram2_depth * mdwidth / 8) - 24 - 16) / 1024;
	nump = min_t(u32, nump, 16);

	 
	reg = dwc3_readl(dwc->regs, DWC3_DCFG);
	reg &= ~DWC3_DCFG_NUMP_MASK;
	reg |= nump << DWC3_DCFG_NUMP_SHIFT;
	dwc3_writel(dwc->regs, DWC3_DCFG, reg);
}