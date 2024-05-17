static void dvb_usb_read_remote_control(struct work_struct *work)
{
	struct dvb_usb_device *d = container_of(work,
			struct dvb_usb_device, rc_query_work.work);
	int ret;

	 
	if (dvb_usbv2_disable_rc_polling || d->rc.bulk_mode) {
		d->rc_polling_active = false;
		return;
	}

	ret = d->rc.query(d);
	if (ret < 0) {
		dev_err(&d->udev->dev, "%s: rc.query() failed=%d\n",
				KBUILD_MODNAME, ret);
		d->rc_polling_active = false;
		return;  
	}

	schedule_delayed_work(&d->rc_query_work,
			msecs_to_jiffies(d->rc.interval));
}
