void __trace_stack(struct trace_array *tr, unsigned long flags, int skip,
		   int pc)
{
	struct ring_buffer *buffer = tr->trace_buffer.buffer;

	if (rcu_is_watching()) {
		__ftrace_trace_stack(buffer, flags, skip, pc, NULL);
		return;
	}

	 
	if (unlikely(in_nmi()))
		return;

	rcu_irq_enter_irqson();
	__ftrace_trace_stack(buffer, flags, skip, pc, NULL);
	rcu_irq_exit_irqson();
}
