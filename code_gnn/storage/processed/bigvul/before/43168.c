static void oz_ep_free(struct oz_port *port, struct oz_endpoint *ep)
{
	if (port) {
		LIST_HEAD(list);
		struct oz_hcd *ozhcd = port->ozhcd;

		if (ep->flags & OZ_F_EP_HAVE_STREAM)
			oz_usb_stream_delete(port->hpd, ep->ep_num);
		 
		spin_lock_bh(&ozhcd->hcd_lock);
		 
		list_replace_init(&ep->urb_list, &list);
		 
		list_splice_tail(&list, &ozhcd->orphanage);
		spin_unlock_bh(&ozhcd->hcd_lock);
	}
	oz_dbg(ON, "Freeing endpoint memory\n");
	kfree(ep);
}