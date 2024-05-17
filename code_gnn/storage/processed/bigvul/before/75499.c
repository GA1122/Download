static bool hub_descriptor_is_sane(struct usb_host_interface *desc)
{
	 
	 
	if (desc->desc.bInterfaceSubClass != 0 &&
	    desc->desc.bInterfaceSubClass != 1)
		return false;

	 
	if (desc->desc.bNumEndpoints != 1)
		return false;

	 
	if (!usb_endpoint_is_int_in(&desc->endpoint[0].desc))
		return false;

        return true;
}
