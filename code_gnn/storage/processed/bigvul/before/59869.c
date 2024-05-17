static int test_halt(struct usbtest_dev *tdev, int ep, struct urb *urb)
{
	int	retval;

	 
	retval = verify_not_halted(tdev, ep, urb);
	if (retval < 0)
		return retval;

	 
	retval = usb_control_msg(urb->dev, usb_sndctrlpipe(urb->dev, 0),
			USB_REQ_SET_FEATURE, USB_RECIP_ENDPOINT,
			USB_ENDPOINT_HALT, ep,
			NULL, 0, USB_CTRL_SET_TIMEOUT);
	if (retval < 0) {
		ERROR(tdev, "ep %02x couldn't set halt, %d\n", ep, retval);
		return retval;
	}
	retval = verify_halted(tdev, ep, urb);
	if (retval < 0) {
		int ret;

		 
		ret = usb_clear_halt(urb->dev, urb->pipe);
		if (ret)
			ERROR(tdev, "ep %02x couldn't clear halt, %d\n",
			      ep, ret);

		return retval;
	}

	 
	retval = usb_clear_halt(urb->dev, urb->pipe);
	if (retval < 0) {
		ERROR(tdev, "ep %02x couldn't clear halt, %d\n", ep, retval);
		return retval;
	}
	retval = verify_not_halted(tdev, ep, urb);
	if (retval < 0)
		return retval;

	 

	return 0;
}
