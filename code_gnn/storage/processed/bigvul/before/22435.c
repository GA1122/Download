static void free_sched_group_rcu(struct rcu_head *rhp)
{
	 
	free_sched_group(container_of(rhp, struct task_group, rcu));
}
