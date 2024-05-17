static void collect_mm_slot(struct mm_slot *mm_slot)
{
	struct mm_struct *mm = mm_slot->mm;

	VM_BUG_ON(!spin_is_locked(&khugepaged_mm_lock));

	if (khugepaged_test_exit(mm)) {
		 
		hlist_del(&mm_slot->hash);
		list_del(&mm_slot->mm_node);

		 

		 
		free_mm_slot(mm_slot);
		mmdrop(mm);
	}
}
