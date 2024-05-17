  unsigned int oom_badness(struct task_struct *p, struct mem_cgroup *mem,
  		      const nodemask_t *nodemask, unsigned long totalpages)
  {
	int points;
// 	long points;
  
  	if (oom_unkillable_task(p, mem, nodemask))
  		return 0;
 
 	p = find_lock_task_mm(p);
 	if (!p)
 		return 0;
 
 	 
 	if (atomic_read(&p->mm->oom_disable_count)) {
 		task_unlock(p);
 		return 0;
 	}
 
 	 
 	if (!totalpages)
 		totalpages = 1;
 
 	 
 	points = get_mm_rss(p->mm) + p->mm->nr_ptes;
 	points += get_mm_counter(p->mm, MM_SWAPENTS);
 
 	points *= 1000;
 	points /= totalpages;
 	task_unlock(p);
 
 	 
 	if (has_capability_noaudit(p, CAP_SYS_ADMIN))
 		points -= 30;
 
 	 
 	points += p->signal->oom_score_adj;
 
 	 
 	if (points <= 0)
 		return 1;
 	return (points < 1000) ? points : 1000;
 }