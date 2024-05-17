struct file *get_mm_exe_file(struct mm_struct *mm)
{
	struct file *exe_file;

	 
	down_read(&mm->mmap_sem);
	exe_file = mm->exe_file;
	if (exe_file)
		get_file(exe_file);
	up_read(&mm->mmap_sem);
	return exe_file;
}
