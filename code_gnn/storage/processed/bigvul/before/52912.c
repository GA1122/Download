static void assign_ctxt_affinity(struct file *fp, struct qib_devdata *dd)
{
	struct qib_filedata *fd = fp->private_data;
	const unsigned int weight = cpumask_weight(&current->cpus_allowed);
	const struct cpumask *local_mask = cpumask_of_pcibus(dd->pcidev->bus);
	int local_cpu;

	 
	if ((weight >= qib_cpulist_count) &&
		(cpumask_weight(local_mask) <= qib_cpulist_count)) {
		for_each_cpu(local_cpu, local_mask)
			if (!test_and_set_bit(local_cpu, qib_cpulist)) {
				fd->rec_cpu_num = local_cpu;
				return;
			}
	}

	 
	if (weight >= qib_cpulist_count) {
		int cpu;

		cpu = find_first_zero_bit(qib_cpulist,
					  qib_cpulist_count);
		if (cpu == qib_cpulist_count)
			qib_dev_err(dd,
			"no cpus avail for affinity PID %u\n",
			current->pid);
		else {
			__set_bit(cpu, qib_cpulist);
			fd->rec_cpu_num = cpu;
		}
	}
}
