static bool vma_has_reserves(struct vm_area_struct *vma, long chg)
{
	if (vma->vm_flags & VM_NORESERVE) {
		 
		if (vma->vm_flags & VM_MAYSHARE && chg == 0)
			return true;
		else
			return false;
	}

	 
	if (vma->vm_flags & VM_MAYSHARE) {
		 
		if (chg)
			return false;
		else
			return true;
	}

	 
	if (is_vma_resv_set(vma, HPAGE_RESV_OWNER)) {
		 
		if (chg)
			return false;
		else
			return true;
	}

	return false;
}
