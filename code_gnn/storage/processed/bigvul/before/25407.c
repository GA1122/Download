static void mt_ase_fp_affinity(void)
{
#ifdef CONFIG_MIPS_MT_FPAFF
	if (mt_fpemul_threshold > 0 &&
	     ((current->thread.emulated_fp++ > mt_fpemul_threshold))) {
		 
		if (cpus_intersects(current->cpus_allowed, mt_fpu_cpumask)) {
			cpumask_t tmask;

			current->thread.user_cpus_allowed
				= current->cpus_allowed;
			cpus_and(tmask, current->cpus_allowed,
				mt_fpu_cpumask);
			set_cpus_allowed_ptr(current, &tmask);
			set_thread_flag(TIF_FPUBOUND);
		}
	}
#endif  
}
