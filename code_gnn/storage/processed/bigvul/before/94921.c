int usbnet_suspend (struct usb_interface *intf, pm_message_t message)
{
	struct usbnet		*dev = usb_get_intfdata(intf);

	if (!dev->suspend_count++) {
		spin_lock_irq(&dev->txq.lock);
		 
		if (dev->txq.qlen && PMSG_IS_AUTO(message)) {
			dev->suspend_count--;
			spin_unlock_irq(&dev->txq.lock);
			return -EBUSY;
		} else {
			set_bit(EVENT_DEV_ASLEEP, &dev->flags);
			spin_unlock_irq(&dev->txq.lock);
		}
		 
		netif_device_detach (dev->net);
		usbnet_terminate_urbs(dev);
		__usbnet_status_stop_force(dev);

		 
		netif_device_attach (dev->net);
	}
	return 0;
}