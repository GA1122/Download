static void ring_buffer_attach(struct perf_event *event,
			       struct ring_buffer *rb)
{
	struct ring_buffer *old_rb = NULL;
	unsigned long flags;

	if (event->rb) {
		 
		WARN_ON_ONCE(event->rcu_pending);

		old_rb = event->rb;
		spin_lock_irqsave(&old_rb->event_lock, flags);
		list_del_rcu(&event->rb_entry);
		spin_unlock_irqrestore(&old_rb->event_lock, flags);

		event->rcu_batches = get_state_synchronize_rcu();
		event->rcu_pending = 1;
	}

	if (rb) {
		if (event->rcu_pending) {
			cond_synchronize_rcu(event->rcu_batches);
			event->rcu_pending = 0;
		}

		spin_lock_irqsave(&rb->event_lock, flags);
		list_add_rcu(&event->rb_entry, &rb->event_list);
		spin_unlock_irqrestore(&rb->event_lock, flags);
	}

	 
	if (has_aux(event))
		perf_event_stop(event, 0);

	rcu_assign_pointer(event->rb, rb);

	if (old_rb) {
		ring_buffer_put(old_rb);
		 
		wake_up_all(&event->waitq);
	}
}