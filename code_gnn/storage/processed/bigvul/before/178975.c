 asmlinkage void do_page_fault(struct pt_regs *regs, unsigned long writeaccess,
 			      unsigned long textaccess, unsigned long address)
 {
 	struct task_struct *tsk;
 	struct mm_struct *mm;
 	struct vm_area_struct * vma;
 	const struct exception_table_entry *fixup;
 	pte_t *pte;
 	int fault;
 
 	 
 
 	tsk = current;
 	mm = tsk->mm;
 
  	 
  	local_irq_enable();
  
	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, 0, regs, address);
// 	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);
  
  	 
 	if (in_atomic() || !mm)
 		goto no_context;
 
 	 
 	down_read(&mm->mmap_sem);
 
 	vma = find_vma(mm, address);
 
 	if (!vma) {
 #ifdef DEBUG_FAULT
 		print_task(tsk);
 		printk("%s:%d fault, address is 0x%08x PC %016Lx textaccess %d writeaccess %d\n",
 		       __func__, __LINE__,
 		       address,regs->pc,textaccess,writeaccess);
 		show_regs(regs);
 #endif
 		goto bad_area;
 	}
 	if (vma->vm_start <= address) {
 		goto good_area;
 	}
 
 	if (!(vma->vm_flags & VM_GROWSDOWN)) {
 #ifdef DEBUG_FAULT
 		print_task(tsk);
 		printk("%s:%d fault, address is 0x%08x PC %016Lx textaccess %d writeaccess %d\n",
 		       __func__, __LINE__,
 		       address,regs->pc,textaccess,writeaccess);
 		show_regs(regs);
 
 		print_vma(vma);
 #endif
 		goto bad_area;
 	}
 	if (expand_stack(vma, address)) {
 #ifdef DEBUG_FAULT
 		print_task(tsk);
 		printk("%s:%d fault, address is 0x%08x PC %016Lx textaccess %d writeaccess %d\n",
 		       __func__, __LINE__,
 		       address,regs->pc,textaccess,writeaccess);
 		show_regs(regs);
 #endif
 		goto bad_area;
 	}
  
 good_area:
 	if (textaccess) {
 		if (!(vma->vm_flags & VM_EXEC))
 			goto bad_area;
 	} else {
 		if (writeaccess) {
 			if (!(vma->vm_flags & VM_WRITE))
 				goto bad_area;
 		} else {
 			if (!(vma->vm_flags & VM_READ))
 				goto bad_area;
 		}
 	}
 
 	 
 	fault = handle_mm_fault(mm, vma, address, writeaccess ? FAULT_FLAG_WRITE : 0);
 	if (unlikely(fault & VM_FAULT_ERROR)) {
 		if (fault & VM_FAULT_OOM)
 			goto out_of_memory;
 		else if (fault & VM_FAULT_SIGBUS)
 			goto do_sigbus;
 		BUG();
 	}
  
  	if (fault & VM_FAULT_MAJOR) {
  		tsk->maj_flt++;
		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1, 0,
// 		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1,
  				     regs, address);
  	} else {
  		tsk->min_flt++;
		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1, 0,
// 		perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1,
  				     regs, address);
  	}
  
 	 
 	pte = lookup_pte (mm, address);
 	if (!pte) {
 		 
 		goto no_pte;
 	}
 
 	__do_tlb_refill(address, textaccess, pte);
 
 no_pte:
 
 	up_read(&mm->mmap_sem);
 	return;
 
  
 bad_area:
 #ifdef DEBUG_FAULT
 	printk("fault:bad area\n");
 #endif
 	up_read(&mm->mmap_sem);
 
 	if (user_mode(regs)) {
 		static int count=0;
 		siginfo_t info;
 		if (count < 4) {
 			 
 			count++;
 			printk("user mode bad_area address=%08lx pid=%d (%s) pc=%08lx\n",
 				address, task_pid_nr(current), current->comm,
 				(unsigned long) regs->pc);
 #if 0
 			show_regs(regs);
 #endif
 		}
 		if (is_global_init(tsk)) {
 			panic("INIT had user mode bad_area\n");
 		}
 		tsk->thread.address = address;
 		tsk->thread.error_code = writeaccess;
 		info.si_signo = SIGSEGV;
 		info.si_errno = 0;
 		info.si_addr = (void *) address;
 		force_sig_info(SIGSEGV, &info, tsk);
 		return;
 	}
 
 no_context:
 #ifdef DEBUG_FAULT
 	printk("fault:No context\n");
 #endif
 	 
 	fixup = search_exception_tables(regs->pc);
 	if (fixup) {
 		regs->pc = fixup->fixup;
 		return;
 	}
 
  
 	if (address < PAGE_SIZE)
 		printk(KERN_ALERT "Unable to handle kernel NULL pointer dereference");
 	else
 		printk(KERN_ALERT "Unable to handle kernel paging request");
 	printk(" at virtual address %08lx\n", address);
 	printk(KERN_ALERT "pc = %08Lx%08Lx\n", regs->pc >> 32, regs->pc & 0xffffffff);
 	die("Oops", regs, writeaccess);
 	do_exit(SIGKILL);
 
  
 out_of_memory:
 	up_read(&mm->mmap_sem);
 	if (!user_mode(regs))
 		goto no_context;
 	pagefault_out_of_memory();
 	return;
 
 do_sigbus:
 	printk("fault:Do sigbus\n");
 	up_read(&mm->mmap_sem);
 
 	 
 	tsk->thread.address = address;
 	tsk->thread.error_code = writeaccess;
 	tsk->thread.trap_no = 14;
 	force_sig(SIGBUS, tsk);
 
 	 
 	if (!user_mode(regs))
 		goto no_context;
 }