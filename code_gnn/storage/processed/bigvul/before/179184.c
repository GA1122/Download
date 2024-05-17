  asmlinkage void bad_mode(struct pt_regs *regs, int reason, unsigned int esr)
  {
// 	siginfo_t info;
// 	void __user *pc = (void __user *)instruction_pointer(regs);
  	console_verbose();
  
  	pr_crit("Bad mode in %s handler detected, code 0x%08x\n",
  		handler[reason], esr);
// 	__show_regs(regs);
// 
// 	info.si_signo = SIGILL;
// 	info.si_errno = 0;
// 	info.si_code  = ILL_ILLOPC;
// 	info.si_addr  = pc;
  
	die("Oops - bad mode", regs, 0);
	local_irq_disable();
	panic("bad mode");
// 	arm64_notify_die("Oops - bad mode", regs, &info, 0);
  }