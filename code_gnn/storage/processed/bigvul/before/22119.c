mm_need_new_owner(struct mm_struct *mm, struct task_struct *p)
{
	 
	if (atomic_read(&mm->mm_users) <= 1)
		return 0;
	if (mm->owner != p)
		return 0;
	return 1;
}
