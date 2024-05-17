static void dwc3_gadget_enable_irq(struct dwc3 *dwc)
{
	u32			reg;

	 
	reg = (DWC3_DEVTEN_VNDRDEVTSTRCVEDEN |
			DWC3_DEVTEN_EVNTOVERFLOWEN |
			DWC3_DEVTEN_CMDCMPLTEN |
			DWC3_DEVTEN_ERRTICERREN |
			DWC3_DEVTEN_WKUPEVTEN |
			DWC3_DEVTEN_CONNECTDONEEN |
			DWC3_DEVTEN_USBRSTEN |
			DWC3_DEVTEN_DISCONNEVTEN);

	if (dwc->revision < DWC3_REVISION_250A)
		reg |= DWC3_DEVTEN_ULSTCNGEN;

	dwc3_writel(dwc->regs, DWC3_DEVTEN, reg);
}