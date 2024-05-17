static void iowarrior_write_callback(struct urb *urb)
{
	struct iowarrior *dev;
	int status = urb->status;

	dev = urb->context;
	 
	if (status &&
	    !(status == -ENOENT ||
	      status == -ECONNRESET || status == -ESHUTDOWN)) {
		dev_dbg(&dev->interface->dev,
			"nonzero write bulk status received: %d\n", status);
	}
	 
	usb_free_coherent(urb->dev, urb->transfer_buffer_length,
			  urb->transfer_buffer, urb->transfer_dma);
	 
	atomic_dec(&dev->write_busy);
	wake_up_interruptible(&dev->write_wait);
}
