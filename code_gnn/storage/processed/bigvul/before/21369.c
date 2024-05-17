int walk_page_range(unsigned long addr, unsigned long end,
		    struct mm_walk *walk)
{
	pgd_t *pgd;
	unsigned long next;
	int err = 0;

	if (addr >= end)
		return err;

	if (!walk->mm)
		return -EINVAL;

	pgd = pgd_offset(walk->mm, addr);
	do {
		struct vm_area_struct *vma;

		next = pgd_addr_end(addr, end);

		 
		vma = hugetlb_vma(addr, walk);
		if (vma) {
			if (vma->vm_end < next)
				next = vma->vm_end;
			 
			err = walk_hugetlb_range(vma, addr, next, walk);
			if (err)
				break;
			pgd = pgd_offset(walk->mm, next);
			continue;
		}

		if (pgd_none_or_clear_bad(pgd)) {
			if (walk->pte_hole)
				err = walk->pte_hole(addr, next, walk);
			if (err)
				break;
			pgd++;
			continue;
		}
		if (walk->pgd_entry)
			err = walk->pgd_entry(pgd, addr, next, walk);
		if (!err &&
		    (walk->pud_entry || walk->pmd_entry || walk->pte_entry))
			err = walk_pud_range(pgd, addr, next, walk);
		if (err)
			break;
		pgd++;
	} while (addr = next, addr != end);

	return err;
}
