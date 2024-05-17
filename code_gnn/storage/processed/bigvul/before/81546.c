update_max_tr_single(struct trace_array *tr, struct task_struct *tsk, int cpu)
{
	int ret;

	if (tr->stop_count)
		return;

	WARN_ON_ONCE(!irqs_disabled());
	if (!tr->allocated_snapshot) {
		 
		WARN_ON_ONCE(tr->current_trace != &nop_trace);
		return;
	}

	arch_spin_lock(&tr->max_lock);

	ret = ring_buffer_swap_cpu(tr->max_buffer.buffer, tr->trace_buffer.buffer, cpu);

	if (ret == -EBUSY) {
		 
		trace_array_printk_buf(tr->max_buffer.buffer, _THIS_IP_,
			"Failed to swap buffers due to commit in progress\n");
	}

	WARN_ON_ONCE(ret && ret != -EAGAIN && ret != -EBUSY);

	__update_max_tr(tr, tsk, cpu);
	arch_spin_unlock(&tr->max_lock);
}
