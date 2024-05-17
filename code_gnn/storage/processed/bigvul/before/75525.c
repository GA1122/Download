int usb_port_resume(struct usb_device *udev, pm_message_t msg)
{
	struct usb_hub	*hub = usb_hub_to_struct_hub(udev->parent);
	struct usb_port *port_dev = hub->ports[udev->portnum  - 1];
	int		port1 = udev->portnum;
	int		status;
	u16		portchange, portstatus;

	if (!test_and_set_bit(port1, hub->child_usage_bits)) {
		status = pm_runtime_get_sync(&port_dev->dev);
		if (status < 0) {
			dev_dbg(&udev->dev, "can't resume usb port, status %d\n",
					status);
			return status;
		}
	}

	usb_lock_port(port_dev);

	 
	status = hub_port_status(hub, port1, &portstatus, &portchange);
	if (status == 0 && !port_is_suspended(hub, portstatus)) {
		if (portchange & USB_PORT_STAT_C_SUSPEND)
			pm_wakeup_event(&udev->dev, 0);
		goto SuspendCleared;
	}

	 
	if (hub_is_superspeed(hub->hdev))
		status = hub_set_port_link_state(hub, port1, USB_SS_PORT_LS_U0);
	else
		status = usb_clear_port_feature(hub->hdev,
				port1, USB_PORT_FEAT_SUSPEND);
	if (status) {
		dev_dbg(&port_dev->dev, "can't resume, status %d\n", status);
	} else {
		 
		dev_dbg(&udev->dev, "usb %sresume\n",
				(PMSG_IS_AUTO(msg) ? "auto-" : ""));
		msleep(USB_RESUME_TIMEOUT);

		 
		status = hub_port_status(hub, port1, &portstatus, &portchange);

		 
		msleep(10);
	}

 SuspendCleared:
	if (status == 0) {
		udev->port_is_suspended = 0;
		if (hub_is_superspeed(hub->hdev)) {
			if (portchange & USB_PORT_STAT_C_LINK_STATE)
				usb_clear_port_feature(hub->hdev, port1,
					USB_PORT_FEAT_C_PORT_LINK_STATE);
		} else {
			if (portchange & USB_PORT_STAT_C_SUSPEND)
				usb_clear_port_feature(hub->hdev, port1,
						USB_PORT_FEAT_C_SUSPEND);
		}
	}

	if (udev->persist_enabled)
		status = wait_for_connected(udev, hub, &port1, &portchange,
				&portstatus);

	status = check_port_resume_type(udev,
			hub, port1, status, portchange, portstatus);
	if (status == 0)
		status = finish_port_resume(udev);
	if (status < 0) {
		dev_dbg(&udev->dev, "can't resume, status %d\n", status);
		hub_port_logical_disconnect(hub, port1);
	} else  {
		 
		if (udev->usb2_hw_lpm_capable == 1)
			usb_set_usb2_hardware_lpm(udev, 1);

		 
		usb_enable_ltm(udev);
	}

	usb_unlock_port(port_dev);

	return status;
}