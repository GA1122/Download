void snd_usb_ctl_msg_quirk(struct usb_device *dev, unsigned int pipe,
			   __u8 request, __u8 requesttype, __u16 value,
			   __u16 index, void *data, __u16 size)
{
	struct snd_usb_audio *chip = dev_get_drvdata(&dev->dev);

	if (!chip)
		return;
	 
	if (USB_ID_VENDOR(chip->usb_id) == 0x23ba &&
	    (requesttype & USB_TYPE_MASK) == USB_TYPE_CLASS)
		mdelay(20);

	 
	if (USB_ID_VENDOR(chip->usb_id) == 0x0644 &&
	    (requesttype & USB_TYPE_MASK) == USB_TYPE_CLASS)
		mdelay(20);

	 
	if (is_marantz_denon_dac(chip->usb_id)
	    && (requesttype & USB_TYPE_MASK) == USB_TYPE_CLASS)
		mdelay(20);

	 
	if (chip->usb_id == USB_ID(0x1686, 0x00dd) &&
	    (requesttype & USB_TYPE_MASK) == USB_TYPE_CLASS)
		mdelay(1);
}
