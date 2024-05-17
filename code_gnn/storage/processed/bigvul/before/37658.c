static void reload_tss(void)
{
	 
	struct desc_ptr *gdt = &__get_cpu_var(host_gdt);
	struct desc_struct *descs;

	descs = (void *)gdt->address;
	descs[GDT_ENTRY_TSS].type = 9;  
	load_TR_desc();
}
