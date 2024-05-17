  void xacct_add_tsk(struct taskstats *stats, struct task_struct *p)
  {
// 	struct mm_struct *mm;
// 
  	 
  	stats->coremem = jiffies_to_usecs(p->acct_rss_mem1) * PAGE_SIZE / MB;
  	stats->virtmem = jiffies_to_usecs(p->acct_vm_mem1) * PAGE_SIZE / MB;
	if (p->mm) {
// 	mm = get_task_mm(p);
// 	if (mm) {
  		 
		stats->hiwater_rss   = p->mm->hiwater_rss * PAGE_SIZE / KB;
		stats->hiwater_vm    = p->mm->hiwater_vm * PAGE_SIZE / KB;
// 		stats->hiwater_rss   = mm->hiwater_rss * PAGE_SIZE / KB;
// 		stats->hiwater_vm    = mm->hiwater_vm * PAGE_SIZE / KB;
// 		mmput(mm);
  	}
  	stats->read_char	= p->rchar;
  	stats->write_char	= p->wchar;
 	stats->read_syscalls	= p->syscr;
 	stats->write_syscalls	= p->syscw;
 }