static int mem_cgroup_move_charge_write(struct cgroup *cgrp,
					struct cftype *cft, u64 val)
{
	struct mem_cgroup *memcg = mem_cgroup_from_cont(cgrp);

	if (val >= (1 << NR_MOVE_TYPE))
		return -EINVAL;
	 
	cgroup_lock();
	memcg->move_charge_at_immigrate = val;
	cgroup_unlock();

	return 0;
}
