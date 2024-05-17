static int rb_cpu_notify(struct notifier_block *self,
			 unsigned long action, void *hcpu)
{
	struct ring_buffer *buffer =
		container_of(self, struct ring_buffer, cpu_notify);
	long cpu = (long)hcpu;
	long nr_pages_same;
	int cpu_i;
	unsigned long nr_pages;

	switch (action) {
	case CPU_UP_PREPARE:
	case CPU_UP_PREPARE_FROZEN:
		if (cpumask_test_cpu(cpu, buffer->cpumask))
			return NOTIFY_OK;

		nr_pages = 0;
		nr_pages_same = 1;
		 
		for_each_buffer_cpu(buffer, cpu_i) {
			 
			if (nr_pages == 0)
				nr_pages = buffer->buffers[cpu_i]->nr_pages;
			if (nr_pages != buffer->buffers[cpu_i]->nr_pages) {
				nr_pages_same = 0;
				break;
			}
		}
		 
		if (!nr_pages_same)
			nr_pages = 2;
		buffer->buffers[cpu] =
			rb_allocate_cpu_buffer(buffer, nr_pages, cpu);
		if (!buffer->buffers[cpu]) {
			WARN(1, "failed to allocate ring buffer on CPU %ld\n",
			     cpu);
			return NOTIFY_OK;
		}
		smp_wmb();
		cpumask_set_cpu(cpu, buffer->cpumask);
		break;
	case CPU_DOWN_PREPARE:
	case CPU_DOWN_PREPARE_FROZEN:
		 
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}
