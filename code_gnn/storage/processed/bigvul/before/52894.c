static void ib_uverbs_free_hw_resources(struct ib_uverbs_device *uverbs_dev,
					struct ib_device *ib_dev)
{
	struct ib_uverbs_file *file;
	struct ib_uverbs_event_file *event_file;
	struct ib_event event;

	 
	synchronize_srcu(&uverbs_dev->disassociate_srcu);
	event.event = IB_EVENT_DEVICE_FATAL;
	event.element.port_num = 0;
	event.device = ib_dev;

	mutex_lock(&uverbs_dev->lists_mutex);
	while (!list_empty(&uverbs_dev->uverbs_file_list)) {
		struct ib_ucontext *ucontext;

		file = list_first_entry(&uverbs_dev->uverbs_file_list,
					struct ib_uverbs_file, list);
		file->is_closed = 1;
		ucontext = file->ucontext;
		list_del(&file->list);
		file->ucontext = NULL;
		kref_get(&file->ref);
		mutex_unlock(&uverbs_dev->lists_mutex);
		 
		ib_uverbs_event_handler(&file->event_handler, &event);
		if (ucontext) {
			ib_dev->disassociate_ucontext(ucontext);
			ib_uverbs_cleanup_ucontext(file, ucontext);
		}

		mutex_lock(&uverbs_dev->lists_mutex);
		kref_put(&file->ref, ib_uverbs_release_file);
	}

	while (!list_empty(&uverbs_dev->uverbs_events_file_list)) {
		event_file = list_first_entry(&uverbs_dev->
					      uverbs_events_file_list,
					      struct ib_uverbs_event_file,
					      list);
		spin_lock_irq(&event_file->lock);
		event_file->is_closed = 1;
		spin_unlock_irq(&event_file->lock);

		list_del(&event_file->list);
		if (event_file->is_async) {
			ib_unregister_event_handler(&event_file->uverbs_file->
						    event_handler);
			event_file->uverbs_file->event_handler.device = NULL;
		}

		wake_up_interruptible(&event_file->poll_wait);
		kill_fasync(&event_file->async_queue, SIGIO, POLL_IN);
	}
	mutex_unlock(&uverbs_dev->lists_mutex);
}