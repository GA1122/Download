int snd_usb_apply_boot_quirk(struct usb_device *dev,
			     struct usb_interface *intf,
			     const struct snd_usb_audio_quirk *quirk,
			     unsigned int id)
{
	switch (id) {
	case USB_ID(0x041e, 0x3000):
		 
		 
		return snd_usb_extigy_boot_quirk(dev, intf);

	case USB_ID(0x041e, 0x3020):
		 
		return snd_usb_audigy2nx_boot_quirk(dev);

	case USB_ID(0x10f5, 0x0200):
		 
		return snd_usb_cm106_boot_quirk(dev);

	case USB_ID(0x0d8c, 0x0102):
		 
	case USB_ID(0x0ccd, 0x00b1):  
		return snd_usb_cm6206_boot_quirk(dev);

	case USB_ID(0x0dba, 0x3000):
		 
		return snd_usb_mbox2_boot_quirk(dev);

	case USB_ID(0x1235, 0x0010):  
	case USB_ID(0x1235, 0x0018):  
		return snd_usb_novation_boot_quirk(dev);

	case USB_ID(0x133e, 0x0815):
		 
		return snd_usb_accessmusic_boot_quirk(dev);

	case USB_ID(0x17cc, 0x1000):  
	case USB_ID(0x17cc, 0x1010):  
	case USB_ID(0x17cc, 0x1020):  
		return snd_usb_nativeinstruments_boot_quirk(dev);
	case USB_ID(0x0763, 0x2012):   
		return snd_usb_fasttrackpro_boot_quirk(dev);
	case USB_ID(0x047f, 0xc010):  
		return snd_usb_gamecon780_boot_quirk(dev);
	}

	return 0;
}
