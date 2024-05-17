static void kill_urbs(struct wdm_device *desc)
{
	 
	usb_kill_urb(desc->command);
	usb_kill_urb(desc->validity);
	usb_kill_urb(desc->response);
}
