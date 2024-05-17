void sched_destroy_group(struct task_group *tg)
{
	 
	call_rcu(&tg->rcu, sched_free_group_rcu);
}
