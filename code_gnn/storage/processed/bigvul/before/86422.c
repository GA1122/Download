static void restore_reserve_on_error(struct hstate *h,
			struct vm_area_struct *vma, unsigned long address,
			struct page *page)
{
	if (unlikely(PagePrivate(page))) {
		long rc = vma_needs_reservation(h, vma, address);

		if (unlikely(rc < 0)) {
			 
			ClearPagePrivate(page);
		} else if (rc) {
			rc = vma_add_reservation(h, vma, address);
			if (unlikely(rc < 0))
				 
				ClearPagePrivate(page);
		} else
			vma_end_reservation(h, vma, address);
	}
}
