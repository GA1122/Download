static void __arpt_unregister_table(struct xt_table *table)
{
	struct xt_table_info *private;
	void *loc_cpu_entry;
	struct module *table_owner = table->me;
	struct arpt_entry *iter;

	private = xt_unregister_table(table);

	 
	loc_cpu_entry = private->entries;
	xt_entry_foreach(iter, loc_cpu_entry, private->size)
		cleanup_entry(iter);
	if (private->number > private->initial_entries)
		module_put(table_owner);
	xt_free_table_info(private);
}