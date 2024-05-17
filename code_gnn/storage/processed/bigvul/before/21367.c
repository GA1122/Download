static int walk_hugetlb_range(struct vm_area_struct *vma,
			      unsigned long addr, unsigned long end,
			      struct mm_walk *walk)
{
	struct hstate *h = hstate_vma(vma);
	unsigned long next;
	unsigned long hmask = huge_page_mask(h);
	pte_t *pte;
	int err = 0;

	do {
		next = hugetlb_entry_end(h, addr, end);
		pte = huge_pte_offset(walk->mm, addr & hmask);
		if (pte && walk->hugetlb_entry)
			err = walk->hugetlb_entry(pte, hmask, addr, next, walk);
		if (err)
			return err;
	} while (addr = next, addr != end);

	return 0;
}
