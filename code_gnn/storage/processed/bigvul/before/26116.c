void perf_event_task_tick(void)
{
	struct list_head *head = &__get_cpu_var(rotation_list);
	struct perf_cpu_context *cpuctx, *tmp;

	WARN_ON(!irqs_disabled());

	list_for_each_entry_safe(cpuctx, tmp, head, rotation_list) {
		if (cpuctx->jiffies_interval == 1 ||
				!(jiffies % cpuctx->jiffies_interval))
			perf_rotate_context(cpuctx);
	}
}
