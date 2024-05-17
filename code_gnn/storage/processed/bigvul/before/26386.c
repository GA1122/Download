int proc_dowatchdog(struct ctl_table *table, int write,
		    void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;

	ret = proc_dointvec_minmax(table, write, buffer, lenp, ppos);
	if (ret || !write)
		goto out;

	if (watchdog_enabled && watchdog_thresh)
		watchdog_enable_all_cpus();
	else
		watchdog_disable_all_cpus();

out:
	return ret;
}
