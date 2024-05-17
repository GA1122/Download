static void __sync_task_rss_stat(struct task_struct *task, struct mm_struct *mm)
{
	int i;

	for (i = 0; i < NR_MM_COUNTERS; i++) {
		if (task->rss_stat.count[i]) {
			add_mm_counter(mm, i, task->rss_stat.count[i]);
			task->rss_stat.count[i] = 0;
		}
	}
	task->rss_stat.events = 0;
}
