static inline void vma_rb_insert(struct vm_area_struct *vma,
				 struct rb_root *root)
{
	 
	validate_mm_rb(root, NULL);

	rb_insert_augmented(&vma->vm_rb, root, &vma_gap_callbacks);
}
