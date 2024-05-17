static void khugepaged_do_scan(struct page **hpage)
{
	unsigned int progress = 0, pass_through_head = 0;
	unsigned int pages = khugepaged_pages_to_scan;

	barrier();  

	while (progress < pages) {
		cond_resched();

#ifndef CONFIG_NUMA
		if (!*hpage) {
			*hpage = alloc_hugepage(khugepaged_defrag());
			if (unlikely(!*hpage)) {
				count_vm_event(THP_COLLAPSE_ALLOC_FAILED);
				break;
			}
			count_vm_event(THP_COLLAPSE_ALLOC);
		}
#else
		if (IS_ERR(*hpage))
			break;
#endif

		if (unlikely(kthread_should_stop() || freezing(current)))
			break;

		spin_lock(&khugepaged_mm_lock);
		if (!khugepaged_scan.mm_slot)
			pass_through_head++;
		if (khugepaged_has_work() &&
		    pass_through_head < 2)
			progress += khugepaged_scan_mm_slot(pages - progress,
							    hpage);
		else
			progress = pages;
		spin_unlock(&khugepaged_mm_lock);
	}
}