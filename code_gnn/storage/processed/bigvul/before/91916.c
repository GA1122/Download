f_midi_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct f_midi *midi = ep->driver_data;
	struct usb_composite_dev *cdev = midi->func.config->cdev;
	int status = req->status;

	switch (status) {
	case 0:			  
		if (ep == midi->out_ep) {
			 
			f_midi_handle_out_data(ep, req);
		} else if (ep == midi->in_ep) {
			 
			req->length = 0;
			f_midi_transmit(midi);
			return;
		}
		break;

	 
	case -ECONNABORTED:	 
	case -ECONNRESET:	 
	case -ESHUTDOWN:	 
		VDBG(cdev, "%s gone (%d), %d/%d\n", ep->name, status,
				req->actual, req->length);
		if (ep == midi->out_ep) {
			f_midi_handle_out_data(ep, req);
			 
			free_ep_req(ep, req);
		}
		return;

	case -EOVERFLOW:	 
	default:
		DBG(cdev, "%s complete --> %d, %d/%d\n", ep->name,
				status, req->actual, req->length);
		break;
	case -EREMOTEIO:	 
		break;
	}

	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		ERROR(cdev, "kill %s:  resubmit %d bytes --> %d\n",
				ep->name, req->length, status);
		usb_ep_set_halt(ep);
		 
	}
}
