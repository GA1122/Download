static int hub_port_wait_reset(struct usb_hub *hub, int port1,
			struct usb_device *udev, unsigned int delay, bool warm)
{
	int delay_time, ret;
	u16 portstatus;
	u16 portchange;
	u32 ext_portstatus = 0;

	for (delay_time = 0;
			delay_time < HUB_RESET_TIMEOUT;
			delay_time += delay) {
		 
		msleep(delay);

		 
		if (hub_is_superspeedplus(hub->hdev))
			ret = hub_ext_port_status(hub, port1,
						  HUB_EXT_PORT_STATUS,
						  &portstatus, &portchange,
						  &ext_portstatus);
		else
			ret = hub_port_status(hub, port1, &portstatus,
					      &portchange);
		if (ret < 0)
			return ret;

		 
		if (!(portstatus & USB_PORT_STAT_RESET) &&
		    (portstatus & USB_PORT_STAT_CONNECTION))
			break;

		 
		if (delay_time >= 2 * HUB_SHORT_RESET_TIME)
			delay = HUB_LONG_RESET_TIME;

		dev_dbg(&hub->ports[port1 - 1]->dev,
				"not %sreset yet, waiting %dms\n",
				warm ? "warm " : "", delay);
	}

	if ((portstatus & USB_PORT_STAT_RESET))
		return -EBUSY;

	if (hub_port_warm_reset_required(hub, port1, portstatus))
		return -ENOTCONN;

	 
	if (!(portstatus & USB_PORT_STAT_CONNECTION))
		return -ENOTCONN;

	 
	if (!hub_is_superspeed(hub->hdev) &&
	    (portchange & USB_PORT_STAT_C_CONNECTION)) {
		usb_clear_port_feature(hub->hdev, port1,
				       USB_PORT_FEAT_C_CONNECTION);
		return -EAGAIN;
	}

	if (!(portstatus & USB_PORT_STAT_ENABLE))
		return -EBUSY;

	if (!udev)
		return 0;

	if (hub_is_superspeedplus(hub->hdev)) {
		 
		udev->rx_lanes = USB_EXT_PORT_RX_LANES(ext_portstatus) + 1;
		udev->tx_lanes = USB_EXT_PORT_TX_LANES(ext_portstatus) + 1;
	} else {
		udev->rx_lanes = 1;
		udev->tx_lanes = 1;
	}
	if (hub_is_wusb(hub))
		udev->speed = USB_SPEED_WIRELESS;
	else if (hub_is_superspeedplus(hub->hdev) &&
		 port_speed_is_ssp(hub->hdev, ext_portstatus &
				   USB_EXT_PORT_STAT_RX_SPEED_ID))
		udev->speed = USB_SPEED_SUPER_PLUS;
	else if (hub_is_superspeed(hub->hdev))
		udev->speed = USB_SPEED_SUPER;
	else if (portstatus & USB_PORT_STAT_HIGH_SPEED)
		udev->speed = USB_SPEED_HIGH;
	else if (portstatus & USB_PORT_STAT_LOW_SPEED)
		udev->speed = USB_SPEED_LOW;
	else
		udev->speed = USB_SPEED_FULL;
	return 0;
}
