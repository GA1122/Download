static void oz_complete_urb(struct usb_hcd *hcd, struct urb *urb,
		int status)
{
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	unsigned long irq_state;
	struct oz_urb_link *cancel_urbl;

	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	usb_hcd_unlink_urb_from_ep(hcd, urb);
	 
	urb->hcpriv = NULL;
	 
	cancel_urbl = oz_uncancel_urb(ozhcd, urb);
	 
	spin_unlock(&g_tasklet_lock);
	if (oz_forget_urb(urb)) {
		oz_dbg(ON, "ERROR Unknown URB %p\n", urb);
	} else {
		atomic_dec(&g_pending_urbs);
		usb_hcd_giveback_urb(hcd, urb, status);
	}
	spin_lock(&g_tasklet_lock);
	spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
	oz_free_urb_link(cancel_urbl);
}
