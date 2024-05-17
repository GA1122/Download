static int khugepaged(void *none)
{
	struct mm_slot *mm_slot;

	set_freezable();
	set_user_nice(current, 19);

	 
	mutex_lock(&khugepaged_mutex);

	for (;;) {
		mutex_unlock(&khugepaged_mutex);
		VM_BUG_ON(khugepaged_thread != current);
		khugepaged_loop();
		VM_BUG_ON(khugepaged_thread != current);

		mutex_lock(&khugepaged_mutex);
		if (!khugepaged_enabled())
			break;
		if (unlikely(kthread_should_stop()))
			break;
	}

	spin_lock(&khugepaged_mm_lock);
	mm_slot = khugepaged_scan.mm_slot;
	khugepaged_scan.mm_slot = NULL;
	if (mm_slot)
		collect_mm_slot(mm_slot);
	spin_unlock(&khugepaged_mm_lock);

	khugepaged_thread = NULL;
	mutex_unlock(&khugepaged_mutex);

	return 0;
}