update_max_tr(struct trace_array *tr, struct task_struct *tsk, int cpu,
	      void *cond_data)
{
	if (tr->stop_count)
		return;

	WARN_ON_ONCE(!irqs_disabled());

	if (!tr->allocated_snapshot) {
		 
		WARN_ON_ONCE(tr->current_trace != &nop_trace);
		return;
	}

	arch_spin_lock(&tr->max_lock);

	 
	if (ring_buffer_record_is_set_on(tr->trace_buffer.buffer))
		ring_buffer_record_on(tr->max_buffer.buffer);
	else
		ring_buffer_record_off(tr->max_buffer.buffer);

#ifdef CONFIG_TRACER_SNAPSHOT
	if (tr->cond_snapshot && !tr->cond_snapshot->update(tr, cond_data))
		goto out_unlock;
#endif
	swap(tr->trace_buffer.buffer, tr->max_buffer.buffer);

	__update_max_tr(tr, tsk, cpu);

 out_unlock:
	arch_spin_unlock(&tr->max_lock);
}
