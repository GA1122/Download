void mmput(struct mm_struct *mm)
{
	might_sleep();

	if (atomic_dec_and_test(&mm->mm_users)) {
		uprobe_clear_state(mm);
		exit_aio(mm);
		ksm_exit(mm);
		khugepaged_exit(mm);  
		exit_mmap(mm);
		set_mm_exe_file(mm, NULL);
		if (!list_empty(&mm->mmlist)) {
			spin_lock(&mmlist_lock);
			list_del(&mm->mmlist);
			spin_unlock(&mmlist_lock);
		}
		if (mm->binfmt)
			module_put(mm->binfmt->module);
		mmdrop(mm);
	}
}