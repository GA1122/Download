static void mincore_page_range(struct vm_area_struct *vma,
			unsigned long addr, unsigned long end,
			unsigned char *vec)
{
	unsigned long next;
	pgd_t *pgd;

	pgd = pgd_offset(vma->vm_mm, addr);
	do {
		next = pgd_addr_end(addr, end);
		if (pgd_none_or_clear_bad(pgd))
			mincore_unmapped_range(vma, addr, next, vec);
		else
			mincore_pud_range(vma, pgd, addr, next, vec);
		vec += (next - addr) >> PAGE_SHIFT;
	} while (pgd++, addr = next, addr != end);
}
