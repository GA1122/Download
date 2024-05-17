static inline bool userfaultfd_must_wait(struct userfaultfd_ctx *ctx,
					 unsigned long address,
					 unsigned long flags,
					 unsigned long reason)
{
	struct mm_struct *mm = ctx->mm;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd, _pmd;
	pte_t *pte;
	bool ret = true;

	VM_BUG_ON(!rwsem_is_locked(&mm->mmap_sem));

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		goto out;
	p4d = p4d_offset(pgd, address);
	if (!p4d_present(*p4d))
		goto out;
	pud = pud_offset(p4d, address);
	if (!pud_present(*pud))
		goto out;
	pmd = pmd_offset(pud, address);
	 
	_pmd = READ_ONCE(*pmd);
	if (pmd_none(_pmd))
		goto out;

	ret = false;
	if (!pmd_present(_pmd))
		goto out;

	if (pmd_trans_huge(_pmd))
		goto out;

	 
	pte = pte_offset_map(pmd, address);
	 
	if (pte_none(*pte))
		ret = true;
	pte_unmap(pte);

out:
	return ret;
}
