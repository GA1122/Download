 asmlinkage void do_sparc_fault(struct pt_regs *regs, int text_fault, int write,
 			       unsigned long address)
 {
 	struct vm_area_struct *vma;
 	struct task_struct *tsk = current;
 	struct mm_struct *mm = tsk->mm;
 	unsigned int fixup;
 	unsigned long g2;
 	int from_user = !(regs->psr & PSR_PS);
 	int fault, code;
 
 	if(text_fault)
 		address = regs->pc;
 
 	 
 	code = SEGV_MAPERR;
 	if (!ARCH_SUN4C && address >= TASK_SIZE)
 		goto vmalloc_fault;
 
 	 
          if (in_atomic() || !mm)
                  goto no_context;
  
	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, 0, regs, address);
// 	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);
  
  	down_read(&mm->mmap_sem);
  
 	 
 	if(!from_user && address >= PAGE_OFFSET)
 		goto bad_area;
 
 	vma = find_vma(mm, address);
 	if(!vma)
 		goto bad_area;
 	if(vma->vm_start <= address)
 		goto good_area;
 	if(!(vma->vm_flags & VM_GROWSDOWN))
 		goto bad_area;
 	if(expand_stack(vma, address))
 		goto bad_area;
 	 
 good_area:
 	code = SEGV_ACCERR;
 	if(write) {
 		if(!(vma->vm_flags & VM_WRITE))
 			goto bad_area;
 	} else {
 		 
 		if(!(vma->vm_flags & (VM_READ | VM_EXEC)))
 			goto bad_area;
 	}
 
 	 
 	fault = handle_mm_fault(mm, vma, address, write ? FAULT_FLAG_WRITE : 0);
 	if (unlikely(fault & VM_FAULT_ERROR)) {
 		if (fault & VM_FAULT_OOM)
 			goto out_of_memory;
 		else if (fault & VM_FAULT_SIGBUS)
 			goto do_sigbus;
 		BUG();
  	}
  	if (fault & VM_FAULT_MAJOR) {
  		current->maj_flt++;
		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1, 0,
			      regs, address);
// 		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1, regs, address);
  	} else {
  		current->min_flt++;
		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1, 0,
			      regs, address);
// 		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1, regs, address);
  	}
  	up_read(&mm->mmap_sem);
  	return;
 
 	 
 bad_area:
 	up_read(&mm->mmap_sem);
 
 bad_area_nosemaphore:
 	 
 	if (from_user) {
 		do_fault_siginfo(code, SIGSEGV, regs, text_fault);
 		return;
 	}
 
 	 
 no_context:
 	g2 = regs->u_regs[UREG_G2];
 	if (!from_user) {
 		fixup = search_extables_range(regs->pc, &g2);
 		if (fixup > 10) {  
 			extern const unsigned __memset_start[];
 			extern const unsigned __memset_end[];
 			extern const unsigned __csum_partial_copy_start[];
 			extern const unsigned __csum_partial_copy_end[];
 
 #ifdef DEBUG_EXCEPTIONS
 			printk("Exception: PC<%08lx> faddr<%08lx>\n", regs->pc, address);
 			printk("EX_TABLE: insn<%08lx> fixup<%08x> g2<%08lx>\n",
 				regs->pc, fixup, g2);
 #endif
 			if ((regs->pc >= (unsigned long)__memset_start &&
 			     regs->pc < (unsigned long)__memset_end) ||
 			    (regs->pc >= (unsigned long)__csum_partial_copy_start &&
 			     regs->pc < (unsigned long)__csum_partial_copy_end)) {
 			        regs->u_regs[UREG_I4] = address;
 				regs->u_regs[UREG_I5] = regs->pc;
 			}
 			regs->u_regs[UREG_G2] = g2;
 			regs->pc = fixup;
 			regs->npc = regs->pc + 4;
 			return;
 		}
 	}
 	
 	unhandled_fault (address, tsk, regs);
 	do_exit(SIGKILL);
 
  
 out_of_memory:
 	up_read(&mm->mmap_sem);
 	if (from_user) {
 		pagefault_out_of_memory();
 		return;
 	}
 	goto no_context;
 
 do_sigbus:
 	up_read(&mm->mmap_sem);
 	do_fault_siginfo(BUS_ADRERR, SIGBUS, regs, text_fault);
 	if (!from_user)
 		goto no_context;
 
 vmalloc_fault:
 	{
 		 
 		int offset = pgd_index(address);
 		pgd_t *pgd, *pgd_k;
 		pmd_t *pmd, *pmd_k;
 
 		pgd = tsk->active_mm->pgd + offset;
 		pgd_k = init_mm.pgd + offset;
 
 		if (!pgd_present(*pgd)) {
 			if (!pgd_present(*pgd_k))
 				goto bad_area_nosemaphore;
 			pgd_val(*pgd) = pgd_val(*pgd_k);
 			return;
 		}
 
 		pmd = pmd_offset(pgd, address);
 		pmd_k = pmd_offset(pgd_k, address);
 
 		if (pmd_present(*pmd) || !pmd_present(*pmd_k))
 			goto bad_area_nosemaphore;
 		*pmd = *pmd_k;
 		return;
 	}
 }