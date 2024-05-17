static int __init setup_relax_domain_level(char *str)
{
	unsigned long val;

	val = simple_strtoul(str, NULL, 0);
	if (val < sched_domain_level_max)
		default_relax_domain_level = val;

	return 1;
}
