static void dwc3_endpoint_transfer_complete(struct dwc3 *dwc,
		struct dwc3_ep *dep, const struct dwc3_event_depevt *event)
{
	unsigned		status = 0;
	int			clean_busy;
	u32			is_xfer_complete;

	is_xfer_complete = (event->endpoint_event == DWC3_DEPEVT_XFERCOMPLETE);

	if (event->status & DEPEVT_STATUS_BUSERR)
		status = -ECONNRESET;

	clean_busy = dwc3_cleanup_done_reqs(dwc, dep, event, status);
	if (clean_busy && (!dep->endpoint.desc || is_xfer_complete ||
				usb_endpoint_xfer_isoc(dep->endpoint.desc)))
		dep->flags &= ~DWC3_EP_BUSY;

	 
	if (dwc->revision < DWC3_REVISION_183A) {
		u32		reg;
		int		i;

		for (i = 0; i < DWC3_ENDPOINTS_NUM; i++) {
			dep = dwc->eps[i];

			if (!(dep->flags & DWC3_EP_ENABLED))
				continue;

			if (!list_empty(&dep->started_list))
				return;
		}

		reg = dwc3_readl(dwc->regs, DWC3_DCTL);
		reg |= dwc->u1u2;
		dwc3_writel(dwc->regs, DWC3_DCTL, reg);

		dwc->u1u2 = 0;
	}

	 
	if (!dep->endpoint.desc)
		return;

	if (!usb_endpoint_xfer_isoc(dep->endpoint.desc))
		__dwc3_gadget_kick_transfer(dep);
}