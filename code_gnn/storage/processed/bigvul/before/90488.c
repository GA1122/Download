static int is_stack(struct vm_area_struct *vma)
{
	 
	return vma->vm_start <= vma->vm_mm->start_stack &&
		vma->vm_end >= vma->vm_mm->start_stack;
}
