void __init trap_init(void)
{
	extern char except_vec3_generic, except_vec3_r4000;
	extern char except_vec4;
	unsigned long i;
	int rollback;

	check_wait();
	rollback = (cpu_wait == r4k_wait);

#if defined(CONFIG_KGDB)
	if (kgdb_early_setup)
		return;	 
#endif

	if (cpu_has_veic || cpu_has_vint) {
		unsigned long size = 0x200 + VECTORSPACING*64;
		ebase = (unsigned long)
			__alloc_bootmem(size, 1 << fls(size), 0);
	} else {
		ebase = CKSEG0;
		if (cpu_has_mips_r2)
			ebase += (read_c0_ebase() & 0x3ffff000);
	}

	per_cpu_trap_init();

	 
	set_handler(0x180, &except_vec3_generic, 0x80);

	 
	for (i = 0; i <= 31; i++)
		set_except_vector(i, handle_reserved);

	 
	if (cpu_has_ejtag && board_ejtag_handler_setup)
		board_ejtag_handler_setup();

	 
	if (cpu_has_watch)
		set_except_vector(23, handle_watch);

	 
	if (cpu_has_veic || cpu_has_vint) {
		int nvec = cpu_has_veic ? 64 : 8;
		for (i = 0; i < nvec; i++)
			set_vi_handler(i, NULL);
	}
	else if (cpu_has_divec)
		set_handler(0x200, &except_vec4, 0x8);

	 
	parity_protection_init();

	 
	if (board_be_init)
		board_be_init();

	set_except_vector(0, rollback ? rollback_handle_int : handle_int);
	set_except_vector(1, handle_tlbm);
	set_except_vector(2, handle_tlbl);
	set_except_vector(3, handle_tlbs);

	set_except_vector(4, handle_adel);
	set_except_vector(5, handle_ades);

	set_except_vector(6, handle_ibe);
	set_except_vector(7, handle_dbe);

	set_except_vector(8, handle_sys);
	set_except_vector(9, handle_bp);
	set_except_vector(10, rdhwr_noopt ? handle_ri :
			  (cpu_has_vtag_icache ?
			   handle_ri_rdhwr_vivt : handle_ri_rdhwr));
	set_except_vector(11, handle_cpu);
	set_except_vector(12, handle_ov);
	set_except_vector(13, handle_tr);

	if (current_cpu_type() == CPU_R6000 ||
	    current_cpu_type() == CPU_R6000A) {
		 
	}


	if (board_nmi_handler_setup)
		board_nmi_handler_setup();

	if (cpu_has_fpu && !cpu_has_nofpuex)
		set_except_vector(15, handle_fpe);

	set_except_vector(22, handle_mdmx);

	if (cpu_has_mcheck)
		set_except_vector(24, handle_mcheck);

	if (cpu_has_mipsmt)
		set_except_vector(25, handle_mt);

	set_except_vector(26, handle_dsp);

	if (cpu_has_vce)
		 
		memcpy((void *)(ebase + 0x180), &except_vec3_r4000, 0x100);
	else if (cpu_has_4kex)
		memcpy((void *)(ebase + 0x180), &except_vec3_generic, 0x80);
	else
		memcpy((void *)(ebase + 0x080), &except_vec3_generic, 0x80);

	local_flush_icache_range(ebase, ebase + 0x400);
	flush_tlb_handlers();

	sort_extable(__start___dbe_table, __stop___dbe_table);

	cu2_notifier(default_cu2_call, 0x80000000);	 
}
