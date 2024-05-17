int mincore_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
		unsigned long addr, unsigned long end,
		unsigned char *vec)
{
	int ret = 0;

	spin_lock(&vma->vm_mm->page_table_lock);
	if (likely(pmd_trans_huge(*pmd))) {
		ret = !pmd_trans_splitting(*pmd);
		spin_unlock(&vma->vm_mm->page_table_lock);
		if (unlikely(!ret))
			wait_split_huge_page(vma->anon_vma, pmd);
		else {
			 
			memset(vec, 1, (end - addr) >> PAGE_SHIFT);
		}
	} else
		spin_unlock(&vma->vm_mm->page_table_lock);

	return ret;
}
