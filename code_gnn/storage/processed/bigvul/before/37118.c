static void kvm_machine_check(void)
{
#if defined(CONFIG_X86_MCE) && defined(CONFIG_X86_64)
	struct pt_regs regs = {
		.cs = 3,  
		.flags = X86_EFLAGS_IF,
	};

	do_machine_check(&regs, 0);
#endif
}