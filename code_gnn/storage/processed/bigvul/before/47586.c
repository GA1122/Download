static void ap_decrease_queue_count(struct ap_device *ap_dev)
{
	int timeout = ap_dev->drv->request_timeout;

	ap_dev->queue_count--;
	if (ap_dev->queue_count > 0)
		mod_timer(&ap_dev->timeout, jiffies + timeout);
	else
		 
		ap_dev->reset = AP_RESET_IGNORE;
}
