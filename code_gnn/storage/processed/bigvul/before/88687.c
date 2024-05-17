static int dwc3_gadget_stop(struct usb_gadget *g)
{
	struct dwc3		*dwc = gadget_to_dwc(g);
	unsigned long		flags;
	int			epnum;
	u32			tmo_eps = 0;

	spin_lock_irqsave(&dwc->lock, flags);

	if (pm_runtime_suspended(dwc->dev))
		goto out;

	__dwc3_gadget_stop(dwc);

	for (epnum = 2; epnum < DWC3_ENDPOINTS_NUM; epnum++) {
		struct dwc3_ep  *dep = dwc->eps[epnum];
		int ret;

		if (!dep)
			continue;

		if (!(dep->flags & DWC3_EP_END_TRANSFER_PENDING))
			continue;

		ret = wait_event_interruptible_lock_irq_timeout(dep->wait_end_transfer,
			    !(dep->flags & DWC3_EP_END_TRANSFER_PENDING),
			    dwc->lock, msecs_to_jiffies(5));

		if (ret <= 0) {
			 
			tmo_eps |= 1 << epnum;
			dep->flags &= DWC3_EP_END_TRANSFER_PENDING;
		}
	}

	if (tmo_eps) {
		dev_err(dwc->dev,
			"end transfer timed out on endpoints 0x%x [bitmap]\n",
			tmo_eps);
	}

out:
	dwc->gadget_driver	= NULL;
	spin_unlock_irqrestore(&dwc->lock, flags);

	free_irq(dwc->irq_gadget, dwc->ev_buf);

	return 0;
}