static void exit_mm(void)
{
	struct mm_struct *mm = current->mm;
	struct core_state *core_state;

	mm_release(current, mm);
	if (!mm)
		return;
	sync_mm_rss(mm);
	 
	down_read(&mm->mmap_sem);
	core_state = mm->core_state;
	if (core_state) {
		struct core_thread self;

		up_read(&mm->mmap_sem);

		self.task = current;
		self.next = xchg(&core_state->dumper.next, &self);
		 
		if (atomic_dec_and_test(&core_state->nr_threads))
			complete(&core_state->startup);

		for (;;) {
			set_current_state(TASK_UNINTERRUPTIBLE);
			if (!self.task)  
				break;
			freezable_schedule();
		}
		__set_current_state(TASK_RUNNING);
		down_read(&mm->mmap_sem);
	}
	mmgrab(mm);
	BUG_ON(mm != current->active_mm);
	 
	task_lock(current);
	current->mm = NULL;
	up_read(&mm->mmap_sem);
	enter_lazy_tlb(mm, current);
	task_unlock(current);
	mm_update_next_owner(mm);
	mmput(mm);
	if (test_thread_flag(TIF_MEMDIE))
		exit_oom_victim();
}
