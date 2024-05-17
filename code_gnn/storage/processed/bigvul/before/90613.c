static __always_inline void vma_rb_erase_ignore(struct vm_area_struct *vma,
						struct rb_root *root,
						struct vm_area_struct *ignore)
{
	 
	validate_mm_rb(root, ignore);

	__vma_rb_erase(vma, root);
}
