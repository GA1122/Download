 static void async_set_reg_cb(struct urb *urb)
{
	struct async_req *req = (struct async_req *)urb->context;
	int status = urb->status;

	if (status < 0)
		dev_dbg(&urb->dev->dev, "%s failed with %d", __func__, status);
	kfree(req);
	usb_free_urb(urb);
}