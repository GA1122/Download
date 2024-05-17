static void decrement_hugepage_resv_vma(struct hstate *h,
			struct vm_area_struct *vma)
{
	if (vma->vm_flags & VM_NORESERVE)
		return;

	if (vma->vm_flags & VM_MAYSHARE) {
		 
		h->resv_huge_pages--;
	} else if (is_vma_resv_set(vma, HPAGE_RESV_OWNER)) {
		 
		h->resv_huge_pages--;
	}
}
