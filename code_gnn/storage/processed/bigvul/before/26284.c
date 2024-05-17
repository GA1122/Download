static void free_sched_domain(struct rcu_head *rcu)
{
	struct sched_domain *sd = container_of(rcu, struct sched_domain, rcu);
	if (atomic_dec_and_test(&sd->groups->ref))
		kfree(sd->groups);
	kfree(sd);
}
