int __pmd_alloc(struct mm_struct *mm, pud_t *pud, unsigned long address)
{
	pmd_t *new = pmd_alloc_one(mm, address);
	if (!new)
		return -ENOMEM;

	smp_wmb();  

	spin_lock(&mm->page_table_lock);
#ifndef __ARCH_HAS_4LEVEL_HACK
	if (pud_present(*pud))		 
		pmd_free(mm, new);
	else
		pud_populate(mm, pud, new);
#else
	if (pgd_present(*pud))		 
		pmd_free(mm, new);
	else
		pgd_populate(mm, pud, new);
#endif  
	spin_unlock(&mm->page_table_lock);
	return 0;
}