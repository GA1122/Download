static void __vma_rb_erase(struct vm_area_struct *vma, struct rb_root *root)
{
	 
	rb_erase_augmented(&vma->vm_rb, root, &vma_gap_callbacks);
}
