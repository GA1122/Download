static void mincore_pte_range(struct vm_area_struct *vma, pmd_t *pmd,
			unsigned long addr, unsigned long end,
			unsigned char *vec)
{
	unsigned long next;
	spinlock_t *ptl;
	pte_t *ptep;

	ptep = pte_offset_map_lock(vma->vm_mm, pmd, addr, &ptl);
	do {
		pte_t pte = *ptep;
		pgoff_t pgoff;

		next = addr + PAGE_SIZE;
		if (pte_none(pte))
			mincore_unmapped_range(vma, addr, next, vec);
		else if (pte_present(pte))
			*vec = 1;
		else if (pte_file(pte)) {
			pgoff = pte_to_pgoff(pte);
			*vec = mincore_page(vma->vm_file->f_mapping, pgoff);
		} else {  
			swp_entry_t entry = pte_to_swp_entry(pte);

			if (is_migration_entry(entry)) {
				 
				*vec = 1;
			} else {
#ifdef CONFIG_SWAP
				pgoff = entry.val;
				*vec = mincore_page(&swapper_space, pgoff);
#else
				WARN_ON(1);
				*vec = 1;
#endif
			}
		}
		vec++;
	} while (ptep++, addr = next, addr != end);
	pte_unmap_unlock(ptep - 1, ptl);
}
