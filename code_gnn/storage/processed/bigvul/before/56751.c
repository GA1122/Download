static void hub_irq(struct urb *urb)
{
	struct usb_hub *hub = urb->context;
	int status = urb->status;
	unsigned i;
	unsigned long bits;

	switch (status) {
	case -ENOENT:		 
	case -ECONNRESET:	 
	case -ESHUTDOWN:	 
		return;

	default:		 
		 
		dev_dbg(hub->intfdev, "transfer --> %d\n", status);
		if ((++hub->nerrors < 10) || hub->error)
			goto resubmit;
		hub->error = status;
		 

	 
	case 0:			 
		bits = 0;
		for (i = 0; i < urb->actual_length; ++i)
			bits |= ((unsigned long) ((*hub->buffer)[i]))
					<< (i*8);
		hub->event_bits[0] = bits;
		break;
	}

	hub->nerrors = 0;

	 
	kick_hub_wq(hub);

resubmit:
	if (hub->quiescing)
		return;

	status = usb_submit_urb(hub->urb, GFP_ATOMIC);
	if (status != 0 && status != -ENODEV && status != -EPERM)
		dev_err(hub->intfdev, "resubmit --> %d\n", status);
}
