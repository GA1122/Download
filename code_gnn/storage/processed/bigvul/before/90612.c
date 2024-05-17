static __always_inline void vma_rb_erase(struct vm_area_struct *vma,
					 struct rb_root *root)
{
	 
	validate_mm_rb(root, vma);

	__vma_rb_erase(vma, root);
}
