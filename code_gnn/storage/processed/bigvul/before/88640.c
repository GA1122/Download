static int dwc3_cleanup_done_reqs(struct dwc3 *dwc, struct dwc3_ep *dep,
		const struct dwc3_event_depevt *event, int status)
{
	struct dwc3_request	*req, *n;
	struct dwc3_trb		*trb;
	bool			ioc = false;
	int			ret = 0;

	list_for_each_entry_safe(req, n, &dep->started_list, list) {
		unsigned length;
		int chain;

		length = req->request.length;
		chain = req->num_pending_sgs > 0;
		if (chain) {
			struct scatterlist *sg = req->sg;
			struct scatterlist *s;
			unsigned int pending = req->num_pending_sgs;
			unsigned int i;

			for_each_sg(sg, s, pending, i) {
				trb = &dep->trb_pool[dep->trb_dequeue];

				if (trb->ctrl & DWC3_TRB_CTRL_HWO)
					break;

				req->sg = sg_next(s);
				req->num_pending_sgs--;

				ret = __dwc3_cleanup_done_trbs(dwc, dep, req, trb,
						event, status, chain);
				if (ret)
					break;
			}
		} else {
			trb = &dep->trb_pool[dep->trb_dequeue];
			ret = __dwc3_cleanup_done_trbs(dwc, dep, req, trb,
					event, status, chain);
		}

		if (req->unaligned || req->zero) {
			trb = &dep->trb_pool[dep->trb_dequeue];
			ret = __dwc3_cleanup_done_trbs(dwc, dep, req, trb,
					event, status, false);
			req->unaligned = false;
			req->zero = false;
		}

		req->request.actual = length - req->remaining;

		if ((req->request.actual < length) && req->num_pending_sgs)
			return __dwc3_gadget_kick_transfer(dep);

		dwc3_gadget_giveback(dep, req, status);

		if (ret) {
			if ((event->status & DEPEVT_STATUS_IOC) &&
			    (trb->ctrl & DWC3_TRB_CTRL_IOC))
				ioc = true;
			break;
		}
	}

	 
	if (!dep->endpoint.desc)
		return 1;

	if (usb_endpoint_xfer_isoc(dep->endpoint.desc) &&
			list_empty(&dep->started_list)) {
		if (list_empty(&dep->pending_list)) {
			 
			dep->flags = DWC3_EP_PENDING_REQUEST;
		} else {
			dwc3_stop_active_transfer(dwc, dep->number, true);
			dep->flags = DWC3_EP_ENABLED;
		}
		return 1;
	}

	if (usb_endpoint_xfer_isoc(dep->endpoint.desc) && ioc)
		return 0;

	return 1;
}