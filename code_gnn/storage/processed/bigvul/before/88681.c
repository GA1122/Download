static void dwc3_gadget_set_speed(struct usb_gadget *g,
				  enum usb_device_speed speed)
{
	struct dwc3		*dwc = gadget_to_dwc(g);
	unsigned long		flags;
	u32			reg;

	spin_lock_irqsave(&dwc->lock, flags);
	reg = dwc3_readl(dwc->regs, DWC3_DCFG);
	reg &= ~(DWC3_DCFG_SPEED_MASK);

	 
	if (dwc->revision < DWC3_REVISION_220A &&
	    !dwc->dis_metastability_quirk) {
		reg |= DWC3_DCFG_SUPERSPEED;
	} else {
		switch (speed) {
		case USB_SPEED_LOW:
			reg |= DWC3_DCFG_LOWSPEED;
			break;
		case USB_SPEED_FULL:
			reg |= DWC3_DCFG_FULLSPEED;
			break;
		case USB_SPEED_HIGH:
			reg |= DWC3_DCFG_HIGHSPEED;
			break;
		case USB_SPEED_SUPER:
			reg |= DWC3_DCFG_SUPERSPEED;
			break;
		case USB_SPEED_SUPER_PLUS:
			if (dwc3_is_usb31(dwc))
				reg |= DWC3_DCFG_SUPERSPEED_PLUS;
			else
				reg |= DWC3_DCFG_SUPERSPEED;
			break;
		default:
			dev_err(dwc->dev, "invalid speed (%d)\n", speed);

			if (dwc->revision & DWC3_REVISION_IS_DWC31)
				reg |= DWC3_DCFG_SUPERSPEED_PLUS;
			else
				reg |= DWC3_DCFG_SUPERSPEED;
		}
	}
	dwc3_writel(dwc->regs, DWC3_DCFG, reg);

	spin_unlock_irqrestore(&dwc->lock, flags);
}