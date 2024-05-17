 void flush_tlb_page(struct vm_area_struct *vma, unsigned long start)
 {
 	struct mm_struct *mm = vma->vm_mm;
 
  	preempt_disable();
  
  	if (current->active_mm == mm) {
		if (current->mm)
// 		if (current->mm) {
// 			 
  			__flush_tlb_one(start);
		else
// 		} else {
  			leave_mm(smp_processor_id());
// 
// 			 
// 			smp_mb();
// 		}
  	}
  
  	if (cpumask_any_but(mm_cpumask(mm), smp_processor_id()) < nr_cpu_ids)
 		flush_tlb_others(mm_cpumask(mm), mm, start, 0UL);
 
 	preempt_enable();
 }