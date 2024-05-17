void scheduler_ipi(void)
{
	sched_ttwu_pending();
}
