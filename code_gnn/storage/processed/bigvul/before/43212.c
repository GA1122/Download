static int oz_urb_process(struct oz_hcd *ozhcd, struct urb *urb)
{
	int rc = 0;
	struct oz_port *port = urb->hcpriv;
	u8 ep_addr;

	 
	oz_remember_urb(urb);
	 
	if (!urb->transfer_buffer && urb->transfer_buffer_length)
		return -EINVAL;
	 
	if ((port->flags & OZ_PORT_F_PRESENT) == 0)
		return -EPIPE;
	ep_addr = usb_pipeendpoint(urb->pipe);
	if (ep_addr) {
		 
		if (oz_enqueue_ep_urb(port, ep_addr, usb_pipein(urb->pipe),
			urb, 0))
			rc = -EPIPE;
	} else {
		oz_process_ep0_urb(ozhcd, urb, GFP_ATOMIC);
	}
	return rc;
}
