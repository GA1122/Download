static void report_wakeup_requests(struct usb_hub *hub)
{
	struct usb_device	*hdev = hub->hdev;
	struct usb_device	*udev;
	struct usb_hcd		*hcd;
	unsigned long		resuming_ports;
	int			i;

	if (hdev->parent)
		return;		 

	hcd = bus_to_hcd(hdev->bus);
	if (hcd->driver->get_resuming_ports) {

		 
		resuming_ports = hcd->driver->get_resuming_ports(hcd);
		for (i = 0; i < hdev->maxchild; ++i) {
			if (test_bit(i, &resuming_ports)) {
				udev = hub->ports[i]->child;
				if (udev)
					pm_wakeup_event(&udev->dev, 0);
			}
		}
	}
}
