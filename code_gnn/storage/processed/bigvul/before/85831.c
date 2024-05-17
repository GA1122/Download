static struct dm_table *__bind(struct mapped_device *md, struct dm_table *t,
			       struct queue_limits *limits)
{
	struct dm_table *old_map;
	struct request_queue *q = md->queue;
	sector_t size;

	lockdep_assert_held(&md->suspend_lock);

	size = dm_table_get_size(t);

	 
	if (size != dm_get_size(md))
		memset(&md->geometry, 0, sizeof(md->geometry));

	__set_size(md, size);

	dm_table_event_callback(t, event_callback, md);

	 
	if (dm_table_request_based(t)) {
		dm_stop_queue(q);
		 
		md->immutable_target = dm_table_get_immutable_target(t);
	}

	__bind_mempools(md, t);

	old_map = rcu_dereference_protected(md->map, lockdep_is_held(&md->suspend_lock));
	rcu_assign_pointer(md->map, (void *)t);
	md->immutable_target_type = dm_table_get_immutable_target_type(t);

	dm_table_set_restrictions(t, q, limits);
	if (old_map)
		dm_sync_table(md);

	return old_map;
}
