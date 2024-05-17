static int dwc3_gadget_ep_dequeue(struct usb_ep *ep,
		struct usb_request *request)
{
	struct dwc3_request		*req = to_dwc3_request(request);
	struct dwc3_request		*r = NULL;

	struct dwc3_ep			*dep = to_dwc3_ep(ep);
	struct dwc3			*dwc = dep->dwc;

	unsigned long			flags;
	int				ret = 0;

	trace_dwc3_ep_dequeue(req);

	spin_lock_irqsave(&dwc->lock, flags);

	list_for_each_entry(r, &dep->pending_list, list) {
		if (r == req)
			break;
	}

	if (r != req) {
		list_for_each_entry(r, &dep->started_list, list) {
			if (r == req)
				break;
		}
		if (r == req) {
			 
			dwc3_stop_active_transfer(dwc, dep->number, true);

			 
			wait_event_lock_irq(dep->wait_end_transfer,
					!(dep->flags & DWC3_EP_END_TRANSFER_PENDING),
					dwc->lock);

			if (!r->trb)
				goto out1;

			if (r->num_pending_sgs) {
				struct dwc3_trb *trb;
				int i = 0;

				for (i = 0; i < r->num_pending_sgs; i++) {
					trb = r->trb + i;
					trb->ctrl &= ~DWC3_TRB_CTRL_HWO;
					dwc3_ep_inc_deq(dep);
				}

				if (r->unaligned || r->zero) {
					trb = r->trb + r->num_pending_sgs + 1;
					trb->ctrl &= ~DWC3_TRB_CTRL_HWO;
					dwc3_ep_inc_deq(dep);
				}
			} else {
				struct dwc3_trb *trb = r->trb;

				trb->ctrl &= ~DWC3_TRB_CTRL_HWO;
				dwc3_ep_inc_deq(dep);

				if (r->unaligned || r->zero) {
					trb = r->trb + 1;
					trb->ctrl &= ~DWC3_TRB_CTRL_HWO;
					dwc3_ep_inc_deq(dep);
				}
			}
			goto out1;
		}
		dev_err(dwc->dev, "request %pK was not queued to %s\n",
				request, ep->name);
		ret = -EINVAL;
		goto out0;
	}

out1:
	 
	dep->queued_requests--;
	dwc3_gadget_giveback(dep, req, -ECONNRESET);

out0:
	spin_unlock_irqrestore(&dwc->lock, flags);

	return ret;
}
