static int __split_huge_page_splitting(struct page *page,
				       struct vm_area_struct *vma,
				       unsigned long address)
{
	struct mm_struct *mm = vma->vm_mm;
	pmd_t *pmd;
	int ret = 0;

	spin_lock(&mm->page_table_lock);
	pmd = page_check_address_pmd(page, mm, address,
				     PAGE_CHECK_ADDRESS_PMD_NOTSPLITTING_FLAG);
	if (pmd) {
		 
		pmdp_splitting_flush_notify(vma, address, pmd);
		ret = 1;
	}
	spin_unlock(&mm->page_table_lock);

	return ret;
}
