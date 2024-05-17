static int validate_group(struct perf_event *event)
{
	struct perf_event *leader = event->group_leader;
	struct cpu_hw_events *fake_cpuc;
	int ret, n;

	ret = -ENOMEM;
	fake_cpuc = kmalloc(sizeof(*fake_cpuc), GFP_KERNEL | __GFP_ZERO);
	if (!fake_cpuc)
		goto out;

	 
	ret = -ENOSPC;
	n = collect_events(fake_cpuc, leader, true);
	if (n < 0)
		goto out_free;

	fake_cpuc->n_events = n;
	n = collect_events(fake_cpuc, event, false);
	if (n < 0)
		goto out_free;

	fake_cpuc->n_events = n;

	ret = x86_pmu.schedule_events(fake_cpuc, n, NULL);

out_free:
	kfree(fake_cpuc);
out:
	return ret;
}