void sync_mm_rss(struct task_struct *task, struct mm_struct *mm)
{
	__sync_task_rss_stat(task, mm);
}
