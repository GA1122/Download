static void vma_gap_update(struct vm_area_struct *vma)
{
	 
	vma_gap_callbacks_propagate(&vma->vm_rb, NULL);
}
