static inline void clear_soft_dirty(struct vm_area_struct *vma,
		unsigned long addr, pte_t *pte)
{
	 
	pte_t ptent = *pte;

	if (pte_present(ptent)) {
		ptent = pte_wrprotect(ptent);
		ptent = pte_clear_flags(ptent, _PAGE_SOFT_DIRTY);
	} else if (is_swap_pte(ptent)) {
		ptent = pte_swp_clear_soft_dirty(ptent);
	}

	set_pte_at(vma->vm_mm, addr, pte, ptent);
}
