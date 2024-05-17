struct file *ib_uverbs_alloc_event_file(struct ib_uverbs_file *uverbs_file,
					struct ib_device	*ib_dev,
					int is_async)
{
	struct ib_uverbs_event_file *ev_file;
	struct file *filp;
	int ret;

	ev_file = kzalloc(sizeof(*ev_file), GFP_KERNEL);
	if (!ev_file)
		return ERR_PTR(-ENOMEM);

	kref_init(&ev_file->ref);
	spin_lock_init(&ev_file->lock);
	INIT_LIST_HEAD(&ev_file->event_list);
	init_waitqueue_head(&ev_file->poll_wait);
	ev_file->uverbs_file = uverbs_file;
	kref_get(&ev_file->uverbs_file->ref);
	ev_file->async_queue = NULL;
	ev_file->is_closed   = 0;

	filp = anon_inode_getfile("[infinibandevent]", &uverbs_event_fops,
				  ev_file, O_RDONLY);
	if (IS_ERR(filp))
		goto err_put_refs;

	mutex_lock(&uverbs_file->device->lists_mutex);
	list_add_tail(&ev_file->list,
		      &uverbs_file->device->uverbs_events_file_list);
	mutex_unlock(&uverbs_file->device->lists_mutex);

	if (is_async) {
		WARN_ON(uverbs_file->async_file);
		uverbs_file->async_file = ev_file;
		kref_get(&uverbs_file->async_file->ref);
		INIT_IB_EVENT_HANDLER(&uverbs_file->event_handler,
				      ib_dev,
				      ib_uverbs_event_handler);
		ret = ib_register_event_handler(&uverbs_file->event_handler);
		if (ret)
			goto err_put_file;

		 
		ev_file->is_async = 1;
	}

	return filp;

err_put_file:
	fput(filp);
	kref_put(&uverbs_file->async_file->ref, ib_uverbs_release_event_file);
	uverbs_file->async_file = NULL;
	return ERR_PTR(ret);

err_put_refs:
	kref_put(&ev_file->uverbs_file->ref, ib_uverbs_release_file);
	kref_put(&ev_file->ref, ib_uverbs_release_event_file);
	return filp;
}