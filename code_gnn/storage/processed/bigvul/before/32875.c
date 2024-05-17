static void dup_mm_exe_file(struct mm_struct *oldmm, struct mm_struct *newmm)
{
	 
	newmm->exe_file = get_mm_exe_file(oldmm);
}
