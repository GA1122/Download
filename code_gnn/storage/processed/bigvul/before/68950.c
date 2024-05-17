void slabinfo_show_stats(struct seq_file *m, struct kmem_cache *cachep)
{
#if STATS
	{			 
		unsigned long high = cachep->high_mark;
		unsigned long allocs = cachep->num_allocations;
		unsigned long grown = cachep->grown;
		unsigned long reaped = cachep->reaped;
		unsigned long errors = cachep->errors;
		unsigned long max_freeable = cachep->max_freeable;
		unsigned long node_allocs = cachep->node_allocs;
		unsigned long node_frees = cachep->node_frees;
		unsigned long overflows = cachep->node_overflow;

		seq_printf(m, " : globalstat %7lu %6lu %5lu %4lu %4lu %4lu %4lu %4lu %4lu",
			   allocs, high, grown,
			   reaped, errors, max_freeable, node_allocs,
			   node_frees, overflows);
	}
	 
	{
		unsigned long allochit = atomic_read(&cachep->allochit);
		unsigned long allocmiss = atomic_read(&cachep->allocmiss);
		unsigned long freehit = atomic_read(&cachep->freehit);
		unsigned long freemiss = atomic_read(&cachep->freemiss);

		seq_printf(m, " : cpustat %6lu %6lu %6lu %6lu",
			   allochit, allocmiss, freehit, freemiss);
	}
#endif
}