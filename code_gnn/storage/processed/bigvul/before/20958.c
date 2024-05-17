static void do_sys_vm86(struct kernel_vm86_struct *info, struct task_struct *tsk)
{
	struct tss_struct *tss;
 
	info->regs.pt.ds = 0;
	info->regs.pt.es = 0;
	info->regs.pt.fs = 0;
#ifndef CONFIG_X86_32_LAZY_GS
	info->regs.pt.gs = 0;
#endif

 
	VEFLAGS = info->regs.pt.flags;
	info->regs.pt.flags &= SAFE_MASK;
	info->regs.pt.flags |= info->regs32->flags & ~SAFE_MASK;
	info->regs.pt.flags |= X86_VM_MASK;

	switch (info->cpu_type) {
	case CPU_286:
		tsk->thread.v86mask = 0;
		break;
	case CPU_386:
		tsk->thread.v86mask = X86_EFLAGS_NT | X86_EFLAGS_IOPL;
		break;
	case CPU_486:
		tsk->thread.v86mask = X86_EFLAGS_AC | X86_EFLAGS_NT | X86_EFLAGS_IOPL;
		break;
	default:
		tsk->thread.v86mask = X86_EFLAGS_ID | X86_EFLAGS_AC | X86_EFLAGS_NT | X86_EFLAGS_IOPL;
		break;
	}

 
	info->regs32->ax = VM86_SIGNAL;
	tsk->thread.saved_sp0 = tsk->thread.sp0;
	tsk->thread.saved_fs = info->regs32->fs;
	tsk->thread.saved_gs = get_user_gs(info->regs32);

	tss = &per_cpu(init_tss, get_cpu());
	tsk->thread.sp0 = (unsigned long) &info->VM86_TSS_ESP0;
	if (cpu_has_sep)
		tsk->thread.sysenter_cs = 0;
	load_sp0(tss, &tsk->thread);
	put_cpu();

	tsk->thread.screen_bitmap = info->screen_bitmap;
	if (info->flags & VM86_SCREEN_BITMAP)
		mark_screen_rdonly(tsk->mm);

	 
#ifdef CONFIG_AUDITSYSCALL
	if (unlikely(current->audit_context))
		__audit_syscall_exit(1, 0);
#endif

	__asm__ __volatile__(
		"movl %0,%%esp\n\t"
		"movl %1,%%ebp\n\t"
#ifdef CONFIG_X86_32_LAZY_GS
		"mov  %2, %%gs\n\t"
#endif
		"jmp resume_userspace"
		:  
		:"r" (&info->regs), "r" (task_thread_info(tsk)), "r" (0));
	 
}
