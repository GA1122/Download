static void vfree_rcu(struct rcu_head *rcu_head)
{
	struct mem_cgroup *memcg;

	memcg = container_of(rcu_head, struct mem_cgroup, rcu_freeing);
	INIT_WORK(&memcg->work_freeing, vfree_work);
	schedule_work(&memcg->work_freeing);
}
