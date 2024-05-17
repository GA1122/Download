context_switch(struct rq *rq, struct task_struct *prev,
	       struct task_struct *next, struct pin_cookie cookie)
{
	struct mm_struct *mm, *oldmm;

	prepare_task_switch(rq, prev, next);

	mm = next->mm;
	oldmm = prev->active_mm;
	 
	arch_start_context_switch(prev);

	if (!mm) {
		next->active_mm = oldmm;
		atomic_inc(&oldmm->mm_count);
		enter_lazy_tlb(oldmm, next);
	} else
		switch_mm_irqs_off(oldmm, mm, next);

	if (!prev->mm) {
		prev->active_mm = NULL;
		rq->prev_mm = oldmm;
	}
	 
	lockdep_unpin_lock(&rq->lock, cookie);
	spin_release(&rq->lock.dep_map, 1, _THIS_IP_);

	 
	switch_to(prev, next, prev);
	barrier();

	return finish_task_switch(prev);
}
