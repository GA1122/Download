void __cpuinit per_cpu_trap_init(void)
{
	unsigned int cpu = smp_processor_id();
	unsigned int status_set = ST0_CU0;
	unsigned int hwrena = cpu_hwrena_impl_bits;
#ifdef CONFIG_MIPS_MT_SMTC
	int secondaryTC = 0;
	int bootTC = (cpu == 0);

	 

	if (((read_c0_tcbind() & TCBIND_CURTC) != 0) &&
	    ((read_c0_tcbind() & TCBIND_CURVPE) == cpu_data[cpu - 1].vpe_id))
		secondaryTC = 1;
#endif  

	 
#ifdef CONFIG_64BIT
	status_set |= ST0_FR|ST0_KX|ST0_SX|ST0_UX;
#endif
	if (current_cpu_data.isa_level == MIPS_CPU_ISA_IV)
		status_set |= ST0_XX;
	if (cpu_has_dsp)
		status_set |= ST0_MX;

	change_c0_status(ST0_CU|ST0_MX|ST0_RE|ST0_FR|ST0_BEV|ST0_TS|ST0_KX|ST0_SX|ST0_UX,
			 status_set);

	if (cpu_has_mips_r2)
		hwrena |= 0x0000000f;

	if (!noulri && cpu_has_userlocal)
		hwrena |= (1 << 29);

	if (hwrena)
		write_c0_hwrena(hwrena);

#ifdef CONFIG_MIPS_MT_SMTC
	if (!secondaryTC) {
#endif  

	if (cpu_has_veic || cpu_has_vint) {
		unsigned long sr = set_c0_status(ST0_BEV);
		write_c0_ebase(ebase);
		write_c0_status(sr);
		 
		change_c0_intctl(0x3e0, VECTORSPACING);
	}
	if (cpu_has_divec) {
		if (cpu_has_mipsmt) {
			unsigned int vpflags = dvpe();
			set_c0_cause(CAUSEF_IV);
			evpe(vpflags);
		} else
			set_c0_cause(CAUSEF_IV);
	}

	 
	if (cpu_has_mips_r2) {
		cp0_compare_irq_shift = CAUSEB_TI - CAUSEB_IP;
		cp0_compare_irq = (read_c0_intctl() >> INTCTLB_IPTI) & 7;
		cp0_perfcount_irq = (read_c0_intctl() >> INTCTLB_IPPCI) & 7;
		if (cp0_perfcount_irq == cp0_compare_irq)
			cp0_perfcount_irq = -1;
	} else {
		cp0_compare_irq = CP0_LEGACY_COMPARE_IRQ;
		cp0_compare_irq_shift = cp0_compare_irq;
		cp0_perfcount_irq = -1;
	}

#ifdef CONFIG_MIPS_MT_SMTC
	}
#endif  

	cpu_data[cpu].asid_cache = ASID_FIRST_VERSION;

	atomic_inc(&init_mm.mm_count);
	current->active_mm = &init_mm;
	BUG_ON(current->mm);
	enter_lazy_tlb(&init_mm, current);

#ifdef CONFIG_MIPS_MT_SMTC
	if (bootTC) {
#endif  
		cpu_cache_init();
		tlb_init();
#ifdef CONFIG_MIPS_MT_SMTC
	} else if (!secondaryTC) {
		 
		write_c0_pagemask(PM_DEFAULT_MASK);
		write_c0_wired(0);
	}
#endif  
	TLBMISS_HANDLER_SETUP();
}
