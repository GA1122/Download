struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
{
	return &gate_vma;
}