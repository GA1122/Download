static void tlb_remove_table_one(void *table)
{
	 
	smp_call_function(tlb_remove_table_smp_sync, NULL, 1);
	__tlb_remove_table(table);
}
