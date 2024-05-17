static int snd_usb_cm106_boot_quirk(struct usb_device *dev)
{
	 
	return snd_usb_cm106_write_int_reg(dev, 2, 0x8004);
}
