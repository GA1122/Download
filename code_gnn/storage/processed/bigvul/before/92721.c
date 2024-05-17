static unsigned int task_scan_max(struct task_struct *p)
{
	unsigned long smin = task_scan_min(p);
	unsigned long smax;

	 
	smax = sysctl_numa_balancing_scan_period_max / task_nr_scan_windows(p);

	 
	if (p->numa_group) {
		struct numa_group *ng = p->numa_group;
		unsigned long shared = group_faults_shared(ng);
		unsigned long private = group_faults_priv(ng);
		unsigned long period = smax;

		period *= atomic_read(&ng->refcount);
		period *= shared + 1;
		period /= private + shared + 1;

		smax = max(smax, period);
	}

	return max(smin, smax);
}
