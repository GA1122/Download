static int usb_string_sub(struct usb_device *dev, unsigned int langid,
			  unsigned int index, unsigned char *buf)
{
	int rc;

	 
	if (dev->quirks & USB_QUIRK_STRING_FETCH_255)
		rc = -EIO;
	else
		rc = usb_get_string(dev, langid, index, buf, 255);

	 
	if (rc < 2) {
		rc = usb_get_string(dev, langid, index, buf, 2);
		if (rc == 2)
			rc = usb_get_string(dev, langid, index, buf, buf[0]);
	}

	if (rc >= 2) {
		if (!buf[0] && !buf[1])
			usb_try_string_workarounds(buf, &rc);

		 
		if (buf[0] < rc)
			rc = buf[0];

		rc = rc - (rc & 1);  
	}

	if (rc < 2)
		rc = (rc < 0 ? rc : -EINVAL);

	return rc;
}