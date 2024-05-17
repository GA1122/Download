static void mincore_pud_range(struct vm_area_struct *vma, pgd_t *pgd,
			unsigned long addr, unsigned long end,
			unsigned char *vec)
{
	unsigned long next;
	pud_t *pud;

	pud = pud_offset(pgd, addr);
	do {
		next = pud_addr_end(addr, end);
		if (pud_none_or_clear_bad(pud))
			mincore_unmapped_range(vma, addr, next, vec);
		else
			mincore_pmd_range(vma, pud, addr, next, vec);
		vec += (next - addr) >> PAGE_SHIFT;
	} while (pud++, addr = next, addr != end);
}
