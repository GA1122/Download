void might_fault(void)
{
	 
	if (segment_eq(get_fs(), KERNEL_DS))
		return;

	might_sleep();
	 
	if (!in_atomic() && current->mm)
		might_lock_read(&current->mm->mmap_sem);
}
