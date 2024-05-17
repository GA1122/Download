struct page *follow_trans_huge_pmd(struct mm_struct *mm,
				   unsigned long addr,
				   pmd_t *pmd,
				   unsigned int flags)
{
	struct page *page = NULL;

	assert_spin_locked(&mm->page_table_lock);

	if (flags & FOLL_WRITE && !pmd_write(*pmd))
		goto out;

	page = pmd_page(*pmd);
	VM_BUG_ON(!PageHead(page));
	if (flags & FOLL_TOUCH) {
		pmd_t _pmd;
		 
		_pmd = pmd_mkyoung(pmd_mkdirty(*pmd));
		set_pmd_at(mm, addr & HPAGE_PMD_MASK, pmd, _pmd);
	}
	page += (addr & ~HPAGE_PMD_MASK) >> PAGE_SHIFT;
	VM_BUG_ON(!PageCompound(page));
	if (flags & FOLL_GET)
		get_page(page);

out:
	return page;
}
