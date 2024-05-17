static void dwc3_endpoint_interrupt(struct dwc3 *dwc,
		const struct dwc3_event_depevt *event)
{
	struct dwc3_ep		*dep;
	u8			epnum = event->endpoint_number;
	u8			cmd;

	dep = dwc->eps[epnum];

	if (!(dep->flags & DWC3_EP_ENABLED)) {
		if (!(dep->flags & DWC3_EP_END_TRANSFER_PENDING))
			return;

		 
		if (event->endpoint_event != DWC3_DEPEVT_EPCMDCMPLT)
			return;
	}

	if (epnum == 0 || epnum == 1) {
		dwc3_ep0_interrupt(dwc, event);
		return;
	}

	switch (event->endpoint_event) {
	case DWC3_DEPEVT_XFERCOMPLETE:
		dep->resource_index = 0;

		if (usb_endpoint_xfer_isoc(dep->endpoint.desc)) {
			dev_err(dwc->dev, "XferComplete for Isochronous endpoint\n");
			return;
		}

		dwc3_endpoint_transfer_complete(dwc, dep, event);
		break;
	case DWC3_DEPEVT_XFERINPROGRESS:
		dwc3_endpoint_transfer_complete(dwc, dep, event);
		break;
	case DWC3_DEPEVT_XFERNOTREADY:
		if (usb_endpoint_xfer_isoc(dep->endpoint.desc))
			dwc3_gadget_start_isoc(dwc, dep, event);
		else
			__dwc3_gadget_kick_transfer(dep);

		break;
	case DWC3_DEPEVT_STREAMEVT:
		if (!usb_endpoint_xfer_bulk(dep->endpoint.desc)) {
			dev_err(dwc->dev, "Stream event for non-Bulk %s\n",
					dep->name);
			return;
		}
		break;
	case DWC3_DEPEVT_EPCMDCMPLT:
		cmd = DEPEVT_PARAMETER_CMD(event->parameters);

		if (cmd == DWC3_DEPCMD_ENDTRANSFER) {
			dep->flags &= ~DWC3_EP_END_TRANSFER_PENDING;
			wake_up(&dep->wait_end_transfer);
		}
		break;
	case DWC3_DEPEVT_RXTXFIFOEVT:
		break;
	}
}