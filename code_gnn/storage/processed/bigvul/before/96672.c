static ssize_t f_hidg_read(struct file *file, char __user *buffer,
			size_t count, loff_t *ptr)
{
	struct f_hidg *hidg = file->private_data;
	struct f_hidg_req_list *list;
	struct usb_request *req;
	unsigned long flags;
	int ret;

	if (!count)
		return 0;

	if (!access_ok(buffer, count))
		return -EFAULT;

	spin_lock_irqsave(&hidg->read_spinlock, flags);

#define READ_COND (!list_empty(&hidg->completed_out_req))

	 
	while (!READ_COND) {
		spin_unlock_irqrestore(&hidg->read_spinlock, flags);
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;

		if (wait_event_interruptible(hidg->read_queue, READ_COND))
			return -ERESTARTSYS;

		spin_lock_irqsave(&hidg->read_spinlock, flags);
	}

	 
	list = list_first_entry(&hidg->completed_out_req,
				struct f_hidg_req_list, list);

	 
	list_del(&list->list);

	req = list->req;
	count = min_t(unsigned int, count, req->actual - list->pos);
	spin_unlock_irqrestore(&hidg->read_spinlock, flags);

	 
	count -= copy_to_user(buffer, req->buf + list->pos, count);
	list->pos += count;

	 
	if (list->pos == req->actual) {
		kfree(list);

		req->length = hidg->report_length;
		ret = usb_ep_queue(hidg->out_ep, req, GFP_KERNEL);
		if (ret < 0) {
			free_ep_req(hidg->out_ep, req);
			return ret;
		}
	} else {
		spin_lock_irqsave(&hidg->read_spinlock, flags);
		list_add(&list->list, &hidg->completed_out_req);
		spin_unlock_irqrestore(&hidg->read_spinlock, flags);

		wake_up(&hidg->read_queue);
	}

	return count;
}
