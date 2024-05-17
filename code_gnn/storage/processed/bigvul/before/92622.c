static void numa_migrate_preferred(struct task_struct *p)
{
	unsigned long interval = HZ;

	 
	if (unlikely(p->numa_preferred_nid == -1 || !p->numa_faults))
		return;

	 
	interval = min(interval, msecs_to_jiffies(p->numa_scan_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	 
	if (task_node(p) == p->numa_preferred_nid)
		return;

	 
	task_numa_migrate(p);
}
