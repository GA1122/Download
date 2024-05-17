static void dwc3_gadget_hibernation_interrupt(struct dwc3 *dwc,
		unsigned int evtinfo)
{
	unsigned int is_ss = evtinfo & BIT(4);

	 
	if (is_ss ^ (dwc->speed == USB_SPEED_SUPER))
		return;

	 
}
