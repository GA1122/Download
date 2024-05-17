static void cypress_write_int_callback(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	struct cypress_private *priv = usb_get_serial_port_data(port);
	struct device *dev = &urb->dev->dev;
	int status = urb->status;

	switch (status) {
	case 0:
		 
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		 
		dev_dbg(dev, "%s - urb shutting down with status: %d\n",
			__func__, status);
		priv->write_urb_in_use = 0;
		return;
	case -EPIPE:
		 
		 
	default:
		dev_err(dev, "%s - unexpected nonzero write status received: %d\n",
			__func__, status);
		cypress_set_dead(port);
		break;
	}
	priv->write_urb_in_use = 0;

	 
	cypress_send(port);
}
