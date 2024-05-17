static struct fsnotify_event *inotify_merge(struct list_head *list,
					    struct fsnotify_event *event)
{
	struct fsnotify_event_holder *last_holder;
	struct fsnotify_event *last_event;

	 
	spin_lock(&event->lock);

	last_holder = list_entry(list->prev, struct fsnotify_event_holder, event_list);
	last_event = last_holder->event;
	if (event_compare(last_event, event))
		fsnotify_get_event(last_event);
	else
		last_event = NULL;

	spin_unlock(&event->lock);

	return last_event;
}