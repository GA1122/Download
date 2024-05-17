static void hub_disconnect_children(struct usb_device *udev)
{
	struct usb_hub *hub = usb_hub_to_struct_hub(udev);
	int i;

	 
	for (i = 0; i < udev->maxchild; i++) {
		if (hub->ports[i]->child)
			usb_disconnect(&hub->ports[i]->child);
	}
}