static ssize_t ffs_epfile_io(struct file *file, struct ffs_io_data *io_data)
{
	struct ffs_epfile *epfile = file->private_data;
	struct usb_request *req;
	struct ffs_ep *ep;
	char *data = NULL;
	ssize_t ret, data_len = -EINVAL;
	int halt;

	 
	if (WARN_ON(epfile->ffs->state != FFS_ACTIVE))
		return -ENODEV;

	 
	ep = epfile->ep;
	if (!ep) {
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;

		ret = wait_event_interruptible(epfile->wait, (ep = epfile->ep));
		if (ret)
			return -EINTR;
	}

	 
	halt = (!io_data->read == !epfile->in);
	if (halt && epfile->isoc)
		return -EINVAL;

	 
	if (!halt) {
		 
		struct usb_gadget *gadget = epfile->ffs->gadget;
		size_t copied;

		spin_lock_irq(&epfile->ffs->eps_lock);
		 
		if (epfile->ep != ep) {
			spin_unlock_irq(&epfile->ffs->eps_lock);
			return -ESHUTDOWN;
		}
		data_len = iov_iter_count(&io_data->data);
		 
		if (io_data->read)
			data_len = usb_ep_align_maybe(gadget, ep->ep, data_len);
		spin_unlock_irq(&epfile->ffs->eps_lock);

		data = kmalloc(data_len, GFP_KERNEL);
		if (unlikely(!data))
			return -ENOMEM;
		if (!io_data->read) {
			copied = copy_from_iter(data, data_len, &io_data->data);
			if (copied != data_len) {
				ret = -EFAULT;
				goto error;
			}
		}
	}

	 
	ret = ffs_mutex_lock(&epfile->mutex, file->f_flags & O_NONBLOCK);
	if (unlikely(ret))
		goto error;

	spin_lock_irq(&epfile->ffs->eps_lock);

	if (epfile->ep != ep) {
		 
		ret = -ESHUTDOWN;
	} else if (halt) {
		 
		if (likely(epfile->ep == ep) && !WARN_ON(!ep->ep))
			usb_ep_set_halt(ep->ep);
		ret = -EBADMSG;
	} else if (unlikely(data_len == -EINVAL)) {
		 
		WARN(1, "%s: data_len == -EINVAL\n", __func__);
		ret = -EINVAL;
	} else if (!io_data->aio) {
		DECLARE_COMPLETION_ONSTACK(done);
		bool interrupted = false;

		req = ep->req;
		req->buf      = data;
		req->length   = data_len;

		req->context  = &done;
		req->complete = ffs_epfile_io_complete;

		ret = usb_ep_queue(ep->ep, req, GFP_ATOMIC);
		if (unlikely(ret < 0))
			goto error_lock;

		spin_unlock_irq(&epfile->ffs->eps_lock);

		if (unlikely(wait_for_completion_interruptible(&done))) {
			 
			usb_ep_dequeue(ep->ep, req);
			interrupted = ep->status < 0;
		}

		 
		ret = interrupted ? -EINTR : ep->status;
		if (io_data->read && ret > 0) {
			ret = copy_to_iter(data, ret, &io_data->data);
			if (!ret)
				ret = -EFAULT;
		}
		goto error_mutex;
	} else if (!(req = usb_ep_alloc_request(ep->ep, GFP_KERNEL))) {
		ret = -ENOMEM;
	} else {
		req->buf      = data;
		req->length   = data_len;

		io_data->buf = data;
		io_data->ep = ep->ep;
		io_data->req = req;
		io_data->ffs = epfile->ffs;

		req->context  = io_data;
		req->complete = ffs_epfile_async_io_complete;

		ret = usb_ep_queue(ep->ep, req, GFP_ATOMIC);
		if (unlikely(ret)) {
			usb_ep_free_request(ep->ep, req);
			goto error_lock;
		}

		ret = -EIOCBQUEUED;
		 
		data = NULL;
	}

error_lock:
	spin_unlock_irq(&epfile->ffs->eps_lock);
error_mutex:
	mutex_unlock(&epfile->mutex);
error:
	kfree(data);
	return ret;
}
