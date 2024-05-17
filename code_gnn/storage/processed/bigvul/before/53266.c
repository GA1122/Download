static int usbfs_increase_memory_usage(unsigned amount)
{
	unsigned lim;

	 
	lim = ACCESS_ONCE(usbfs_memory_mb);
	if (lim == 0 || lim > (USBFS_XFER_MAX >> 20))
		lim = USBFS_XFER_MAX;
	else
		lim <<= 20;

	atomic_add(amount, &usbfs_memory_usage);
	if (atomic_read(&usbfs_memory_usage) <= lim)
		return 0;
	atomic_sub(amount, &usbfs_memory_usage);
	return -ENOMEM;
}
