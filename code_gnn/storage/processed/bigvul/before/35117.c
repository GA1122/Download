static inline int khugepaged_test_exit(struct mm_struct *mm)
{
	return atomic_read(&mm->mm_users) == 0;
}