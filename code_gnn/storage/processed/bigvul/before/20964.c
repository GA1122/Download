int handle_vm86_trap(struct kernel_vm86_regs *regs, long error_code, int trapno)
{
	if (VMPI.is_vm86pus) {
		if ((trapno == 3) || (trapno == 1)) {
			KVM86->regs32->ax = VM86_TRAP + (trapno << 8);
			 
			set_thread_flag(TIF_IRET);
			return 0;
		}
		do_int(regs, trapno, (unsigned char __user *) (regs->pt.ss << 4), SP(regs));
		return 0;
	}
	if (trapno != 1)
		return 1;  
	current->thread.trap_no = trapno;
	current->thread.error_code = error_code;
	force_sig(SIGTRAP, current);
	return 0;
}