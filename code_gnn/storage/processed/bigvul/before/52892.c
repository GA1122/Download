static ssize_t ib_uverbs_event_read(struct file *filp, char __user *buf,
				    size_t count, loff_t *pos)
{
	struct ib_uverbs_event_file *file = filp->private_data;
	struct ib_uverbs_event *event;
	int eventsz;
	int ret = 0;

	spin_lock_irq(&file->lock);

	while (list_empty(&file->event_list)) {
		spin_unlock_irq(&file->lock);

		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		if (wait_event_interruptible(file->poll_wait,
					     (!list_empty(&file->event_list) ||
			 
					     !file->uverbs_file->device->ib_dev)))
			return -ERESTARTSYS;

		 
		if (list_empty(&file->event_list) &&
		    !file->uverbs_file->device->ib_dev)
			return -EIO;

		spin_lock_irq(&file->lock);
	}

	event = list_entry(file->event_list.next, struct ib_uverbs_event, list);

	if (file->is_async)
		eventsz = sizeof (struct ib_uverbs_async_event_desc);
	else
		eventsz = sizeof (struct ib_uverbs_comp_event_desc);

	if (eventsz > count) {
		ret   = -EINVAL;
		event = NULL;
	} else {
		list_del(file->event_list.next);
		if (event->counter) {
			++(*event->counter);
			list_del(&event->obj_list);
		}
	}

	spin_unlock_irq(&file->lock);

	if (event) {
		if (copy_to_user(buf, event, eventsz))
			ret = -EFAULT;
		else
			ret = eventsz;
	}

	kfree(event);

	return ret;
}