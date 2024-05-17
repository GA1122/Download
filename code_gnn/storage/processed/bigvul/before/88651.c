static void dwc3_gadget_disable_irq(struct dwc3 *dwc)
{
	 
	dwc3_writel(dwc->regs, DWC3_DEVTEN, 0x00);
}
