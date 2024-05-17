static void rfcomm_dev_state_change(struct rfcomm_dlc *dlc, int err)
{
	struct rfcomm_dev *dev = dlc->owner;
	if (!dev)
		return;

	BT_DBG("dlc %p dev %p err %d", dlc, dev, err);

	dev->err = err;
	wake_up_interruptible(&dev->wait);

	if (dlc->state == BT_CLOSED) {
		if (!dev->port.tty) {
			if (test_bit(RFCOMM_RELEASE_ONHUP, &dev->flags)) {
				 
				rfcomm_dlc_unlock(dlc);
				if (rfcomm_dev_get(dev->id) == NULL) {
					rfcomm_dlc_lock(dlc);
					return;
				}

				rfcomm_dev_del(dev);
				tty_port_put(&dev->port);
				rfcomm_dlc_lock(dlc);
			}
		} else
			tty_hangup(dev->port.tty);
	}
}
