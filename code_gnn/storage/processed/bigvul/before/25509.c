void __kprobes do_asce_exception(struct pt_regs *regs, long pgm_int_code,
				 unsigned long trans_exc_code)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	if (unlikely(!user_space_fault(trans_exc_code) || in_atomic() || !mm))
		goto no_context;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, trans_exc_code & __FAIL_ADDR_MASK);
	up_read(&mm->mmap_sem);

	if (vma) {
		update_mm(mm, current);
		return;
	}

	 
	if (regs->psw.mask & PSW_MASK_PSTATE) {
		do_sigsegv(regs, pgm_int_code, SEGV_MAPERR, trans_exc_code);
		return;
	}

no_context:
	do_no_context(regs, pgm_int_code, trans_exc_code);
}
