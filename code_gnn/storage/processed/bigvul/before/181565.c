 static enum led_brightness k90_backlight_get(struct led_classdev *led_cdev)
 {
 	int ret;
 	struct k90_led *led = container_of(led_cdev, struct k90_led, cdev);
 	struct device *dev = led->cdev.dev->parent;
  	struct usb_interface *usbif = to_usb_interface(dev->parent);
  	struct usb_device *usbdev = interface_to_usbdev(usbif);
  	int brightness;
	char data[8];
// 	char *data;
// 
// 	data = kmalloc(8, GFP_KERNEL);
// 	if (!data)
// 		return -ENOMEM;
  
  	ret = usb_control_msg(usbdev, usb_rcvctrlpipe(usbdev, 0),
  			      K90_REQUEST_STATUS,
 			      USB_DIR_IN | USB_TYPE_VENDOR |
 			      USB_RECIP_DEVICE, 0, 0, data, 8,
 			      USB_CTRL_SET_TIMEOUT);
  	if (ret < 0) {
  		dev_warn(dev, "Failed to get K90 initial state (error %d).\n",
  			 ret);
		return -EIO;
// 		ret = -EIO;
// 		goto out;
  	}
  	brightness = data[4];
  	if (brightness < 0 || brightness > 3) {
  		dev_warn(dev,
  			 "Read invalid backlight brightness: %02hhx.\n",
  			 data[4]);
		return -EIO;
// 		ret = -EIO;
// 		goto out;
  	}
	return brightness;
// 	ret = brightness;
// out:
// 	kfree(data);
// 
// 	return ret;
  }