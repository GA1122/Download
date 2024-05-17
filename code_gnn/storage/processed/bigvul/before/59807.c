static void hid_irq_out(struct urb *urb)
{
	struct hid_device *hid = urb->context;
	struct usbhid_device *usbhid = hid->driver_data;
	unsigned long flags;
	int unplug = 0;

	switch (urb->status) {
	case 0:			 
		break;
	case -ESHUTDOWN:	 
		unplug = 1;
	case -EILSEQ:		 
	case -EPROTO:		 
	case -ECONNRESET:	 
	case -ENOENT:
		break;
	default:		 
		hid_warn(urb->dev, "output irq status %d received\n",
			 urb->status);
	}

	spin_lock_irqsave(&usbhid->lock, flags);

	if (unplug) {
		usbhid->outtail = usbhid->outhead;
	} else {
		usbhid->outtail = (usbhid->outtail + 1) & (HID_OUTPUT_FIFO_SIZE - 1);

		if (usbhid->outhead != usbhid->outtail &&
				hid_submit_out(hid) == 0) {
			 
			spin_unlock_irqrestore(&usbhid->lock, flags);
			return;
		}
	}

	clear_bit(HID_OUT_RUNNING, &usbhid->iofl);
	spin_unlock_irqrestore(&usbhid->lock, flags);
	usb_autopm_put_interface_async(usbhid->intf);
	wake_up(&usbhid->wait);
}
