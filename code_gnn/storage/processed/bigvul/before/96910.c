void trace_buffered_event_disable(void)
{
	int cpu;

	WARN_ON_ONCE(!mutex_is_locked(&event_mutex));

	if (WARN_ON_ONCE(!trace_buffered_event_ref))
		return;

	if (--trace_buffered_event_ref)
		return;

	preempt_disable();
	 
	smp_call_function_many(tracing_buffer_mask,
			       disable_trace_buffered_event, NULL, 1);
	preempt_enable();

	 
	synchronize_rcu();

	for_each_tracing_cpu(cpu) {
		free_page((unsigned long)per_cpu(trace_buffered_event, cpu));
		per_cpu(trace_buffered_event, cpu) = NULL;
	}
	 
	smp_wmb();

	preempt_disable();
	 
	smp_call_function_many(tracing_buffer_mask,
			       enable_trace_buffered_event, NULL, 1);
	preempt_enable();
}
