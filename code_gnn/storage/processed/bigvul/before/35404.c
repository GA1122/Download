void show_regs(struct pt_regs *regs)
{
	int i;
	unsigned long sp;

	sp = regs->sp;
	show_regs_print_info(KERN_DEFAULT);
	__show_regs(regs, 1);

	 
	if (!user_mode(regs)) {
		unsigned int code_prologue = code_bytes * 43 / 64;
		unsigned int code_len = code_bytes;
		unsigned char c;
		u8 *ip;

		printk(KERN_DEFAULT "Stack:\n");
		show_stack_log_lvl(NULL, regs, (unsigned long *)sp,
				   0, KERN_DEFAULT);

		printk(KERN_DEFAULT "Code: ");

		ip = (u8 *)regs->ip - code_prologue;
		if (ip < (u8 *)PAGE_OFFSET || probe_kernel_address(ip, c)) {
			 
			ip = (u8 *)regs->ip;
			code_len = code_len - code_prologue + 1;
		}
		for (i = 0; i < code_len; i++, ip++) {
			if (ip < (u8 *)PAGE_OFFSET ||
					probe_kernel_address(ip, c)) {
				pr_cont(" Bad RIP value.");
				break;
			}
			if (ip == (u8 *)regs->ip)
				pr_cont("<%02x> ", c);
			else
				pr_cont("%02x ", c);
		}
	}
	pr_cont("\n");
}