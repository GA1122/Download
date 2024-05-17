void release_task(struct task_struct * p)
{
	struct task_struct *leader;
	int zap_leader;
repeat:
	tracehook_prepare_release_task(p);
	 
	atomic_dec(&__task_cred(p)->user->processes);

	proc_flush_task(p);

	write_lock_irq(&tasklist_lock);
	tracehook_finish_release_task(p);
	__exit_signal(p);

	 
	zap_leader = 0;
	leader = p->group_leader;
	if (leader != p && thread_group_empty(leader) && leader->exit_state == EXIT_ZOMBIE) {
		BUG_ON(task_detached(leader));
		do_notify_parent(leader, leader->exit_signal);
		 
		zap_leader = task_detached(leader);

		 
		if (zap_leader)
			leader->exit_state = EXIT_DEAD;
	}

	write_unlock_irq(&tasklist_lock);
	release_thread(p);
	call_rcu(&p->rcu, delayed_put_task_struct);

	p = leader;
	if (unlikely(zap_leader))
		goto repeat;
}
