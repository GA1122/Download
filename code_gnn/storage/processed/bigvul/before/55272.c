static int snd_usb_novation_boot_quirk(struct usb_device *dev)
{
	 
	usb_set_interface(dev, 0, 1);
	return 0;
}
