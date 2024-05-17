static int snd_usb_nativeinstruments_boot_quirk(struct usb_device *dev)
{
	int ret = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
				  0xaf, USB_TYPE_VENDOR | USB_RECIP_DEVICE,
				  1, 0, NULL, 0, 1000);

	if (ret < 0)
		return ret;

	usb_reset_device(dev);

	 
	return -EAGAIN;
}
