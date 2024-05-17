static void split_huge_page_address(struct mm_struct *mm,
				    unsigned long address)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	VM_BUG_ON(!(address & ~HPAGE_PMD_MASK));

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return;
	 
	split_huge_page_pmd(mm, pmd);
}
