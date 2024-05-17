static inline void unmap_mapping_range_list(struct list_head *head,
					    struct zap_details *details)
{
	struct vm_area_struct *vma;

	 
	list_for_each_entry(vma, head, shared.nonlinear) {
		details->nonlinear_vma = vma;
		unmap_mapping_range_vma(vma, vma->vm_start, vma->vm_end, details);
	}
}