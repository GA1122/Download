void __init trap_init(void)
{
	 
	setup_cpu_entry_areas();

	idt_setup_traps();

	 
	cea_set_pte(CPU_ENTRY_AREA_RO_IDT_VADDR, __pa_symbol(idt_table),
		    PAGE_KERNEL_RO);
	idt_descr.address = CPU_ENTRY_AREA_RO_IDT;

	 
	cpu_init();

	idt_setup_ist_traps();

	x86_init.irqs.trap_init();

	idt_setup_debugidt_traps();
}
