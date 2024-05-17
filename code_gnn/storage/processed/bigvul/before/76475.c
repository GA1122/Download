static __always_inline ssize_t mfill_atomic_pte(struct mm_struct *dst_mm,
						pmd_t *dst_pmd,
						struct vm_area_struct *dst_vma,
						unsigned long dst_addr,
						unsigned long src_addr,
						struct page **page,
						bool zeropage)
{
	ssize_t err;

	 
	if (!(dst_vma->vm_flags & VM_SHARED)) {
		if (!zeropage)
			err = mcopy_atomic_pte(dst_mm, dst_pmd, dst_vma,
					       dst_addr, src_addr, page);
		else
			err = mfill_zeropage_pte(dst_mm, dst_pmd,
						 dst_vma, dst_addr);
	} else {
		if (!zeropage)
			err = shmem_mcopy_atomic_pte(dst_mm, dst_pmd,
						     dst_vma, dst_addr,
						     src_addr, page);
		else
			err = shmem_mfill_zeropage_pte(dst_mm, dst_pmd,
						       dst_vma, dst_addr);
	}

	return err;
}
