static int oz_hcd_urb_dequeue(struct usb_hcd *hcd, struct urb *urb, int status)
{
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	struct oz_urb_link *urbl;
	int rc;
	unsigned long irq_state;

	oz_dbg(URB, "%s: (%p)\n",  __func__, urb);
	urbl = oz_alloc_urb_link();
	if (unlikely(urbl == NULL))
		return -ENOMEM;
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	 
	rc = usb_hcd_check_unlink_urb(hcd, urb, status);
	 
	if ((rc == 0) && urb->hcpriv) {
		urbl->urb = urb;
		urbl->port = (struct oz_port *)urb->hcpriv;
		urbl->ep_num = usb_pipeendpoint(urb->pipe);
		if (usb_pipein(urb->pipe))
			urbl->ep_num |= USB_DIR_IN;
		list_add_tail(&urbl->link, &ozhcd->urb_cancel_list);
		spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
		tasklet_schedule(&g_urb_cancel_tasklet);
	} else {
		spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
		oz_free_urb_link(urbl);
	}
	return rc;
}