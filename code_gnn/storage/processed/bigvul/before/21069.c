bool mem_cgroup_handle_oom(struct mem_cgroup *memcg, gfp_t mask)
{
	struct oom_wait_info owait;
	bool locked, need_to_kill;

	owait.mem = memcg;
	owait.wait.flags = 0;
	owait.wait.func = memcg_oom_wake_function;
	owait.wait.private = current;
	INIT_LIST_HEAD(&owait.wait.task_list);
	need_to_kill = true;
	mem_cgroup_mark_under_oom(memcg);

	 
	spin_lock(&memcg_oom_lock);
	locked = mem_cgroup_oom_lock(memcg);
	 
	prepare_to_wait(&memcg_oom_waitq, &owait.wait, TASK_KILLABLE);
	if (!locked || memcg->oom_kill_disable)
		need_to_kill = false;
	if (locked)
		mem_cgroup_oom_notify(memcg);
	spin_unlock(&memcg_oom_lock);

	if (need_to_kill) {
		finish_wait(&memcg_oom_waitq, &owait.wait);
		mem_cgroup_out_of_memory(memcg, mask);
	} else {
		schedule();
		finish_wait(&memcg_oom_waitq, &owait.wait);
	}
	spin_lock(&memcg_oom_lock);
	if (locked)
		mem_cgroup_oom_unlock(memcg);
	memcg_wakeup_oom(memcg);
	spin_unlock(&memcg_oom_lock);

	mem_cgroup_unmark_under_oom(memcg);

	if (test_thread_flag(TIF_MEMDIE) || fatal_signal_pending(current))
		return false;
	 
	schedule_timeout_uninterruptible(1);
	return true;
}