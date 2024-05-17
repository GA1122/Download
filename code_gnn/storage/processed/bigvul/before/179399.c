 int khugepaged_enter_vma_merge(struct vm_area_struct *vma)
 {
 	unsigned long hstart, hend;
 	if (!vma->anon_vma)
 		 
  		return 0;
	if (vma->vm_file || vma->vm_ops)
// 	if (vma->vm_ops)
  		 
  		return 0;
	VM_BUG_ON(is_linear_pfn_mapping(vma) || is_pfn_mapping(vma));
// 	 
// 	VM_BUG_ON(is_linear_pfn_mapping(vma) || vma->vm_flags & VM_NO_THP);
  	hstart = (vma->vm_start + ~HPAGE_PMD_MASK) & HPAGE_PMD_MASK;
  	hend = vma->vm_end & HPAGE_PMD_MASK;
  	if (hstart < hend)
 		return khugepaged_enter(vma);
 	return 0;
 }