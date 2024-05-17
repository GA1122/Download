static void intr_callback(struct urb *urb)
{
	rtl8150_t *dev;
	__u8 *d;
	int status = urb->status;
	int res;

	dev = urb->context;
	if (!dev)
		return;
	switch (status) {
	case 0:			 
		break;
	case -ECONNRESET:	 
	case -ENOENT:
	case -ESHUTDOWN:
		return;
	 
	default:
		dev_info(&urb->dev->dev, "%s: intr status %d\n",
			 dev->netdev->name, status);
		goto resubmit;
	}

	d = urb->transfer_buffer;
	if (d[0] & TSR_ERRORS) {
		dev->netdev->stats.tx_errors++;
		if (d[INT_TSR] & (TSR_ECOL | TSR_JBR))
			dev->netdev->stats.tx_aborted_errors++;
		if (d[INT_TSR] & TSR_LCOL)
			dev->netdev->stats.tx_window_errors++;
		if (d[INT_TSR] & TSR_LOSS_CRS)
			dev->netdev->stats.tx_carrier_errors++;
	}
	 
	if ((d[INT_MSR] & MSR_LINK) == 0) {
		if (netif_carrier_ok(dev->netdev)) {
			netif_carrier_off(dev->netdev);
			netdev_dbg(dev->netdev, "%s: LINK LOST\n", __func__);
		}
	} else {
		if (!netif_carrier_ok(dev->netdev)) {
			netif_carrier_on(dev->netdev);
			netdev_dbg(dev->netdev, "%s: LINK CAME BACK\n", __func__);
		}
	}

resubmit:
	res = usb_submit_urb (urb, GFP_ATOMIC);
	if (res == -ENODEV)
		netif_device_detach(dev->netdev);
	else if (res)
		dev_err(&dev->udev->dev,
			"can't resubmit intr, %s-%s/input0, status %d\n",
			dev->udev->bus->bus_name, dev->udev->devpath, res);
}