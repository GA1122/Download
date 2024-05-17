static int omninet_attach(struct usb_serial *serial)
{
	 
	if (serial->num_bulk_out < 2) {
		dev_err(&serial->interface->dev, "missing endpoints\n");
		return -ENODEV;
	}

	return 0;
}
