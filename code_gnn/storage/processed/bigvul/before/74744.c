void system_reset_exception(struct pt_regs *regs)
{
	 
	if (ppc_md.system_reset_exception) {
		if (ppc_md.system_reset_exception(regs))
			return;
	}

#ifdef CONFIG_KEXEC
	cpu_set(smp_processor_id(), cpus_in_sr);
#endif

	die("System Reset", regs, SIGABRT);

	 
	crash_kexec_secondary(regs);

	 
	if (!(regs->msr & MSR_RI))
		panic("Unrecoverable System Reset");

	 
}
