static void cancel_async_set_config(struct usb_device *udev)
{
	struct set_config_request *req;

	spin_lock(&set_config_lock);
	list_for_each_entry(req, &set_config_list, node) {
		if (req->udev == udev)
			req->config = -999;	 
	}
	spin_unlock(&set_config_lock);
}
