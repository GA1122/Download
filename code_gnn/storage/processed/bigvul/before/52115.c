static int gup_pte_range(pmd_t pmd, unsigned long addr, unsigned long end,
			 int write, struct page **pages, int *nr)
{
	pte_t *ptep, *ptem;
	int ret = 0;

	ptem = ptep = pte_offset_map(&pmd, addr);
	do {
		 
		pte_t pte = READ_ONCE(*ptep);
		struct page *head, *page;

		 
		if (!pte_present(pte) || pte_special(pte) ||
			pte_protnone(pte) || (write && !pte_write(pte)))
			goto pte_unmap;

		if (!arch_pte_access_permitted(pte, write))
			goto pte_unmap;

		VM_BUG_ON(!pfn_valid(pte_pfn(pte)));
		page = pte_page(pte);
		head = compound_head(page);

		if (!page_cache_get_speculative(head))
			goto pte_unmap;

		if (unlikely(pte_val(pte) != pte_val(*ptep))) {
			put_page(head);
			goto pte_unmap;
		}

		VM_BUG_ON_PAGE(compound_head(page) != head, page);
		pages[*nr] = page;
		(*nr)++;

	} while (ptep++, addr += PAGE_SIZE, addr != end);

	ret = 1;

pte_unmap:
	pte_unmap(ptem);
	return ret;
}
