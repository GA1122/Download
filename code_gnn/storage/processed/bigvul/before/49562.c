static ssize_t __ffs_ep0_read_events(struct ffs_data *ffs, char __user *buf,
				     size_t n)
{
	 
	struct usb_functionfs_event events[ARRAY_SIZE(ffs->ev.types)];
	const size_t size = n * sizeof *events;
	unsigned i = 0;

	memset(events, 0, size);

	do {
		events[i].type = ffs->ev.types[i];
		if (events[i].type == FUNCTIONFS_SETUP) {
			events[i].u.setup = ffs->ev.setup;
			ffs->setup_state = FFS_SETUP_PENDING;
		}
	} while (++i < n);

	ffs->ev.count -= n;
	if (ffs->ev.count)
		memmove(ffs->ev.types, ffs->ev.types + n,
			ffs->ev.count * sizeof *ffs->ev.types);

	spin_unlock_irq(&ffs->ev.waitq.lock);
	mutex_unlock(&ffs->mutex);

	return unlikely(copy_to_user(buf, events, size)) ? -EFAULT : size;
}
