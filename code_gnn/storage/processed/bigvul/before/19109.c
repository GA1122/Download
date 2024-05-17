void __init udp_table_init(struct udp_table *table, const char *name)
{
	unsigned int i;

	if (!CONFIG_BASE_SMALL)
		table->hash = alloc_large_system_hash(name,
			2 * sizeof(struct udp_hslot),
			uhash_entries,
			21,  
			0,
			&table->log,
			&table->mask,
			64 * 1024);
	 
	if (CONFIG_BASE_SMALL || table->mask < UDP_HTABLE_SIZE_MIN - 1) {
		table->hash = kmalloc(UDP_HTABLE_SIZE_MIN *
				      2 * sizeof(struct udp_hslot), GFP_KERNEL);
		if (!table->hash)
			panic(name);
		table->log = ilog2(UDP_HTABLE_SIZE_MIN);
		table->mask = UDP_HTABLE_SIZE_MIN - 1;
	}
	table->hash2 = table->hash + (table->mask + 1);
	for (i = 0; i <= table->mask; i++) {
		INIT_HLIST_NULLS_HEAD(&table->hash[i].head, i);
		table->hash[i].count = 0;
		spin_lock_init(&table->hash[i].lock);
	}
	for (i = 0; i <= table->mask; i++) {
		INIT_HLIST_NULLS_HEAD(&table->hash2[i].head, i);
		table->hash2[i].count = 0;
		spin_lock_init(&table->hash2[i].lock);
	}
}