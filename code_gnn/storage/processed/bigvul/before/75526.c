int usb_port_suspend(struct usb_device *udev, pm_message_t msg)
{
	struct usb_hub	*hub = usb_hub_to_struct_hub(udev->parent);
	struct usb_port *port_dev = hub->ports[udev->portnum - 1];
	int		port1 = udev->portnum;
	int		status;
	bool		really_suspend = true;

	usb_lock_port(port_dev);

	 
	if (udev->do_remote_wakeup) {
		status = usb_enable_remote_wakeup(udev);
		if (status) {
			dev_dbg(&udev->dev, "won't remote wakeup, status %d\n",
					status);
			 
			if (PMSG_IS_AUTO(msg))
				goto err_wakeup;
		}
	}

	 
	if (udev->usb2_hw_lpm_enabled == 1)
		usb_set_usb2_hardware_lpm(udev, 0);

	if (usb_disable_ltm(udev)) {
		dev_err(&udev->dev, "Failed to disable LTM before suspend\n");
		status = -ENOMEM;
		if (PMSG_IS_AUTO(msg))
			goto err_ltm;
	}

	 
	if (hub_is_superspeed(hub->hdev))
		status = hub_set_port_link_state(hub, port1, USB_SS_PORT_LS_U3);

	 
	else if (PMSG_IS_AUTO(msg) || wakeup_enabled_descendants(udev) > 0)
		status = set_port_feature(hub->hdev, port1,
				USB_PORT_FEAT_SUSPEND);
	else {
		really_suspend = false;
		status = 0;
	}
	if (status) {
		dev_dbg(&port_dev->dev, "can't suspend, status %d\n", status);

		 
		usb_enable_ltm(udev);
 err_ltm:
		 
		if (udev->usb2_hw_lpm_capable == 1)
			usb_set_usb2_hardware_lpm(udev, 1);

		if (udev->do_remote_wakeup)
			(void) usb_disable_remote_wakeup(udev);
 err_wakeup:

		 
		if (!PMSG_IS_AUTO(msg))
			status = 0;
	} else {
		dev_dbg(&udev->dev, "usb %ssuspend, wakeup %d\n",
				(PMSG_IS_AUTO(msg) ? "auto-" : ""),
				udev->do_remote_wakeup);
		if (really_suspend) {
			udev->port_is_suspended = 1;

			 
			msleep(10);
		}
		usb_set_device_state(udev, USB_STATE_SUSPENDED);
	}

	if (status == 0 && !udev->do_remote_wakeup && udev->persist_enabled
			&& test_and_clear_bit(port1, hub->child_usage_bits))
		pm_runtime_put_sync(&port_dev->dev);

	usb_mark_last_busy(hub->hdev);

	usb_unlock_port(port_dev);
	return status;
}
