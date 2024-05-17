static void digi_disconnect(struct usb_serial *serial)
{
	int i;

	 
	for (i = 0; i < serial->type->num_ports + 1; i++) {
		usb_kill_urb(serial->port[i]->read_urb);
		usb_kill_urb(serial->port[i]->write_urb);
	}
}
