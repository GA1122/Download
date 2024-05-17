static bool always_dump_vma(struct vm_area_struct *vma)
{
	 
	if (vma == get_gate_vma(vma->vm_mm))
		return true;

	 
	if (vma->vm_ops && vma->vm_ops->name && vma->vm_ops->name(vma))
		return true;

	 
	if (arch_vma_name(vma))
		return true;

	return false;
}
