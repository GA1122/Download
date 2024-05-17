void snd_usb_set_interface_quirk(struct usb_device *dev)
{
	struct snd_usb_audio *chip = dev_get_drvdata(&dev->dev);

	if (!chip)
		return;
	 
	switch (USB_ID_VENDOR(chip->usb_id)) {
	case 0x23ba:  
	case 0x0644:  
		mdelay(50);
		break;
	}
}
