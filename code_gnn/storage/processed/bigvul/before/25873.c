int kgdb_arch_init(void)
{
 	return register_die_notifier(&kgdb_notifier);
 }