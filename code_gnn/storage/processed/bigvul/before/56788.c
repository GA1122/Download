static void update_devnum(struct usb_device *udev, int devnum)
{
	 
	if (!udev->wusb)
		udev->devnum = devnum;
}
