static void sg_complete(struct urb *urb)
{
	struct usb_sg_request *io = urb->context;
	int status = urb->status;

	spin_lock(&io->lock);

	 
	if (io->status
			&& (io->status != -ECONNRESET
				|| status != -ECONNRESET)
			&& urb->actual_length) {
		dev_err(io->dev->bus->controller,
			"dev %s ep%d%s scatterlist error %d/%d\n",
			io->dev->devpath,
			usb_endpoint_num(&urb->ep->desc),
			usb_urb_dir_in(urb) ? "in" : "out",
			status, io->status);
		 
	}

	if (io->status == 0 && status && status != -ECONNRESET) {
		int i, found, retval;

		io->status = status;

		 
		spin_unlock(&io->lock);
		for (i = 0, found = 0; i < io->entries; i++) {
			if (!io->urbs[i])
				continue;
			if (found) {
				usb_block_urb(io->urbs[i]);
				retval = usb_unlink_urb(io->urbs[i]);
				if (retval != -EINPROGRESS &&
				    retval != -ENODEV &&
				    retval != -EBUSY &&
				    retval != -EIDRM)
					dev_err(&io->dev->dev,
						"%s, unlink --> %d\n",
						__func__, retval);
			} else if (urb == io->urbs[i])
				found = 1;
		}
		spin_lock(&io->lock);
	}

	 
	io->bytes += urb->actual_length;
	io->count--;
	if (!io->count)
		complete(&io->complete);

	spin_unlock(&io->lock);
}
