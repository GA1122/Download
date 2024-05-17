asmlinkage void do_cpu(struct pt_regs *regs)
{
	unsigned int __user *epc;
	unsigned long old_epc;
	unsigned int opcode;
	unsigned int cpid;
	int status;
	unsigned long __maybe_unused flags;

	die_if_kernel("do_cpu invoked from kernel context!", regs);

	cpid = (regs->cp0_cause >> CAUSEB_CE) & 3;

	switch (cpid) {
	case 0:
		epc = (unsigned int __user *)exception_epc(regs);
		old_epc = regs->cp0_epc;
		opcode = 0;
		status = -1;

		if (unlikely(compute_return_epc(regs) < 0))
			return;

		if (unlikely(get_user(opcode, epc) < 0))
			status = SIGSEGV;

		if (!cpu_has_llsc && status < 0)
			status = simulate_llsc(regs, opcode);

		if (status < 0)
			status = simulate_rdhwr(regs, opcode);

		if (status < 0)
			status = SIGILL;

		if (unlikely(status > 0)) {
			regs->cp0_epc = old_epc;	 
			force_sig(status, current);
		}

		return;

	case 1:
		if (used_math())	 
			own_fpu(1);
		else {			 
			init_fpu();
			set_used_math();
		}

		if (!raw_cpu_has_fpu) {
			int sig;
			void __user *fault_addr = NULL;
			sig = fpu_emulator_cop1Handler(regs,
						       &current->thread.fpu,
						       0, &fault_addr);
			if (!process_fpemu_return(sig, fault_addr))
				mt_ase_fp_affinity();
		}

		return;

	case 2:
		raw_notifier_call_chain(&cu2_chain, CU2_EXCEPTION, regs);
		return;

	case 3:
		break;
	}

	force_sig(SIGILL, current);
}