static int treo_attach(struct usb_serial *serial)
{
	struct usb_serial_port *swap_port;

	 
	if (!((le16_to_cpu(serial->dev->descriptor.idVendor)
						== HANDSPRING_VENDOR_ID) ||
		(le16_to_cpu(serial->dev->descriptor.idVendor)
						== KYOCERA_VENDOR_ID)) ||
		(serial->num_interrupt_in == 0))
		return 0;

	if (serial->num_bulk_in < 2 || serial->num_interrupt_in < 2) {
		dev_err(&serial->interface->dev, "missing endpoints\n");
		return -ENODEV;
	}

	 
#define COPY_PORT(dest, src)						\
	do { \
		int i;							\
									\
		for (i = 0; i < ARRAY_SIZE(src->read_urbs); ++i) {	\
			dest->read_urbs[i] = src->read_urbs[i];		\
			dest->read_urbs[i]->context = dest;		\
			dest->bulk_in_buffers[i] = src->bulk_in_buffers[i]; \
		}							\
		dest->read_urb = src->read_urb;				\
		dest->bulk_in_endpointAddress = src->bulk_in_endpointAddress;\
		dest->bulk_in_buffer = src->bulk_in_buffer;		\
		dest->bulk_in_size = src->bulk_in_size;			\
		dest->interrupt_in_urb = src->interrupt_in_urb;		\
		dest->interrupt_in_urb->context = dest;			\
		dest->interrupt_in_endpointAddress = \
					src->interrupt_in_endpointAddress;\
		dest->interrupt_in_buffer = src->interrupt_in_buffer;	\
	} while (0);

	swap_port = kmalloc(sizeof(*swap_port), GFP_KERNEL);
	if (!swap_port)
		return -ENOMEM;
	COPY_PORT(swap_port, serial->port[0]);
	COPY_PORT(serial->port[0], serial->port[1]);
	COPY_PORT(serial->port[1], swap_port);
	kfree(swap_port);

	return 0;
}
