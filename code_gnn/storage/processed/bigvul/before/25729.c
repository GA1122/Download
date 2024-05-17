asmlinkage void do_sun4c_fault(struct pt_regs *regs, int text_fault, int write,
			       unsigned long address)
{
	extern void sun4c_update_mmu_cache(struct vm_area_struct *,
					   unsigned long,pte_t *);
	extern pte_t *sun4c_pte_offset_kernel(pmd_t *,unsigned long);
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;
	pgd_t *pgdp;
	pte_t *ptep;

	if (text_fault) {
		address = regs->pc;
	} else if (!write &&
		   !(regs->psr & PSR_PS)) {
		unsigned int insn, __user *ip;

		ip = (unsigned int __user *)regs->pc;
		if (!get_user(insn, ip)) {
			if ((insn & 0xc1680000) == 0xc0680000)
				write = 1;
		}
	}

	if (!mm) {
		 
		do_sparc_fault(regs, text_fault, write, address);
		BUG();	 
	}

	pgdp = pgd_offset(mm, address);
	ptep = sun4c_pte_offset_kernel((pmd_t *) pgdp, address);

	if (pgd_val(*pgdp)) {
	    if (write) {
		if ((pte_val(*ptep) & (_SUN4C_PAGE_WRITE|_SUN4C_PAGE_PRESENT))
				   == (_SUN4C_PAGE_WRITE|_SUN4C_PAGE_PRESENT)) {
			unsigned long flags;

			*ptep = __pte(pte_val(*ptep) | _SUN4C_PAGE_ACCESSED |
				      _SUN4C_PAGE_MODIFIED |
				      _SUN4C_PAGE_VALID |
				      _SUN4C_PAGE_DIRTY);

			local_irq_save(flags);
			if (sun4c_get_segmap(address) != invalid_segment) {
				sun4c_put_pte(address, pte_val(*ptep));
				local_irq_restore(flags);
				return;
			}
			local_irq_restore(flags);
		}
	    } else {
		if ((pte_val(*ptep) & (_SUN4C_PAGE_READ|_SUN4C_PAGE_PRESENT))
				   == (_SUN4C_PAGE_READ|_SUN4C_PAGE_PRESENT)) {
			unsigned long flags;

			*ptep = __pte(pte_val(*ptep) | _SUN4C_PAGE_ACCESSED |
				      _SUN4C_PAGE_VALID);

			local_irq_save(flags);
			if (sun4c_get_segmap(address) != invalid_segment) {
				sun4c_put_pte(address, pte_val(*ptep));
				local_irq_restore(flags);
				return;
			}
			local_irq_restore(flags);
		}
	    }
	}

	 
	if (pgd_val(*pgdp) && !(write && !(pte_val(*ptep) & _SUN4C_PAGE_WRITE))
	    && (pte_val(*ptep) & _SUN4C_PAGE_VALID))
		 
		sun4c_update_mmu_cache (find_vma(current->mm, address), address,
					ptep);
	else
		do_sparc_fault(regs, text_fault, write, address);
}
