  static int uas_switch_interface(struct usb_device *udev,
  				struct usb_interface *intf)
  {
	int alt;
// 	struct usb_host_interface *alt;
  
  	alt = uas_find_uas_alt_setting(intf);
	if (alt < 0)
		return alt;
// 	if (!alt)
// 		return -ENODEV;
  
	return usb_set_interface(udev,
			intf->altsetting[0].desc.bInterfaceNumber, alt);
// 	return usb_set_interface(udev, alt->desc.bInterfaceNumber,
// 			alt->desc.bAlternateSetting);
  }