static void usb_enable_link_state(struct usb_hcd *hcd, struct usb_device *udev,
		enum usb3_link_state state)
{
	int timeout, ret;
	__u8 u1_mel = udev->bos->ss_cap->bU1devExitLat;
	__le16 u2_mel = udev->bos->ss_cap->bU2DevExitLat;

	 
	if ((state == USB3_LPM_U1 && u1_mel == 0) ||
			(state == USB3_LPM_U2 && u2_mel == 0))
		return;

	 
	ret = usb_req_set_sel(udev, state);
	if (ret < 0) {
		dev_warn(&udev->dev, "Set SEL for device-initiated %s failed.\n",
				usb3_lpm_names[state]);
		return;
	}

	 
	timeout = hcd->driver->enable_usb3_lpm_timeout(hcd, udev, state);

	 
	if (timeout == 0)
		return;

	if (timeout < 0) {
		dev_warn(&udev->dev, "Could not enable %s link state, "
				"xHCI error %i.\n", usb3_lpm_names[state],
				timeout);
		return;
	}

	if (usb_set_lpm_timeout(udev, state, timeout)) {
		 
		hcd->driver->disable_usb3_lpm_timeout(hcd, udev, state);
	} else {
		 
		if (udev->actconfig)
			usb_set_device_initiated_lpm(udev, state, true);

		 
		if (state == USB3_LPM_U1)
			udev->usb3_lpm_u1_enabled = 1;
		else if (state == USB3_LPM_U2)
			udev->usb3_lpm_u2_enabled = 1;
	}
}
