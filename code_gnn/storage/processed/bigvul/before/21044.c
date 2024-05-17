static void mem_cgroup_clear_mc(void)
{
	struct mem_cgroup *from = mc.from;

	 
	mc.moving_task = NULL;
	__mem_cgroup_clear_mc();
	spin_lock(&mc.lock);
	mc.from = NULL;
	mc.to = NULL;
	spin_unlock(&mc.lock);
	mem_cgroup_end_move(from);
}
