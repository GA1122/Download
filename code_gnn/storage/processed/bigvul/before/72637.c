int ring_buffer_wait(struct ring_buffer *buffer, int cpu, bool full)
{
	struct ring_buffer_per_cpu *uninitialized_var(cpu_buffer);
	DEFINE_WAIT(wait);
	struct rb_irq_work *work;
	int ret = 0;

	 
	if (cpu == RING_BUFFER_ALL_CPUS) {
		work = &buffer->irq_work;
		 
		full = false;
	} else {
		if (!cpumask_test_cpu(cpu, buffer->cpumask))
			return -ENODEV;
		cpu_buffer = buffer->buffers[cpu];
		work = &cpu_buffer->irq_work;
	}


	while (true) {
		if (full)
			prepare_to_wait(&work->full_waiters, &wait, TASK_INTERRUPTIBLE);
		else
			prepare_to_wait(&work->waiters, &wait, TASK_INTERRUPTIBLE);

		 
		if (full)
			work->full_waiters_pending = true;
		else
			work->waiters_pending = true;

		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}

		if (cpu == RING_BUFFER_ALL_CPUS && !ring_buffer_empty(buffer))
			break;

		if (cpu != RING_BUFFER_ALL_CPUS &&
		    !ring_buffer_empty_cpu(buffer, cpu)) {
			unsigned long flags;
			bool pagebusy;

			if (!full)
				break;

			raw_spin_lock_irqsave(&cpu_buffer->reader_lock, flags);
			pagebusy = cpu_buffer->reader_page == cpu_buffer->commit_page;
			raw_spin_unlock_irqrestore(&cpu_buffer->reader_lock, flags);

			if (!pagebusy)
				break;
		}

		schedule();
	}

	if (full)
		finish_wait(&work->full_waiters, &wait);
	else
		finish_wait(&work->waiters, &wait);

	return ret;
}
