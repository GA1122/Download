static void perf_group_detach(struct perf_event *event)
{
	struct perf_event *sibling, *tmp;
	struct list_head *list = NULL;

	lockdep_assert_held(&event->ctx->lock);

	 
	if (!(event->attach_state & PERF_ATTACH_GROUP))
		return;

	event->attach_state &= ~PERF_ATTACH_GROUP;

	 
	if (event->group_leader != event) {
		list_del_init(&event->group_entry);
		event->group_leader->nr_siblings--;
		goto out;
	}

	if (!list_empty(&event->group_entry))
		list = &event->group_entry;

	 
	list_for_each_entry_safe(sibling, tmp, &event->sibling_list, group_entry) {
		if (list)
			list_move_tail(&sibling->group_entry, list);
		sibling->group_leader = sibling;

		 
		sibling->group_caps = event->group_caps;

		WARN_ON_ONCE(sibling->ctx != event->ctx);
	}

out:
	perf_event__header_size(event->group_leader);

	list_for_each_entry(tmp, &event->group_leader->sibling_list, group_entry)
		perf_event__header_size(tmp);
}
