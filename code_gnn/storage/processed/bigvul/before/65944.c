svc_pool_map_set_cpumask(struct task_struct *task, unsigned int pidx)
{
	struct svc_pool_map *m = &svc_pool_map;
	unsigned int node = m->pool_to[pidx];

	 
	WARN_ON_ONCE(m->count == 0);
	if (m->count == 0)
		return;

	switch (m->mode) {
	case SVC_POOL_PERCPU:
	{
		set_cpus_allowed_ptr(task, cpumask_of(node));
		break;
	}
	case SVC_POOL_PERNODE:
	{
		set_cpus_allowed_ptr(task, cpumask_of_node(node));
		break;
	}
	}
}