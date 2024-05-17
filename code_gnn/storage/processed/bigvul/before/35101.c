pgtable_t get_pmd_huge_pte(struct mm_struct *mm)
{
	pgtable_t pgtable;

	assert_spin_locked(&mm->page_table_lock);

	 
	pgtable = mm->pmd_huge_pte;
	if (list_empty(&pgtable->lru))
		mm->pmd_huge_pte = NULL;
	else {
		mm->pmd_huge_pte = list_entry(pgtable->lru.next,
					      struct page, lru);
		list_del(&pgtable->lru);
	}
	return pgtable;
}