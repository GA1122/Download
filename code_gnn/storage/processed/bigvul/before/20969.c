struct pt_regs *save_v86_state(struct kernel_vm86_regs *regs)
{
	struct tss_struct *tss;
	struct pt_regs *ret;
	unsigned long tmp;

	 
	local_irq_enable();

	if (!current->thread.vm86_info) {
		printk("no vm86_info: BAD\n");
		do_exit(SIGSEGV);
	}
	set_flags(regs->pt.flags, VEFLAGS, X86_EFLAGS_VIF | current->thread.v86mask);
	tmp = copy_vm86_regs_to_user(&current->thread.vm86_info->regs, regs);
	tmp += put_user(current->thread.screen_bitmap, &current->thread.vm86_info->screen_bitmap);
	if (tmp) {
		printk("vm86: could not access userspace vm86_info\n");
		do_exit(SIGSEGV);
	}

	tss = &per_cpu(init_tss, get_cpu());
	current->thread.sp0 = current->thread.saved_sp0;
	current->thread.sysenter_cs = __KERNEL_CS;
	load_sp0(tss, &current->thread);
	current->thread.saved_sp0 = 0;
	put_cpu();

	ret = KVM86->regs32;

	ret->fs = current->thread.saved_fs;
	set_user_gs(ret, current->thread.saved_gs);

	return ret;
}