static int alloc_kmem_cache_cpus(struct kmem_cache *s, gfp_t flags)
{
	int cpu;

	for_each_online_cpu(cpu) {
		struct kmem_cache_cpu *c = get_cpu_slab(s, cpu);

		if (c)
			continue;

		c = alloc_kmem_cache_cpu(s, cpu, flags);
		if (!c) {
			free_kmem_cache_cpus(s);
			return 0;
		}
		s->cpu_slab[cpu] = c;
	}
	return 1;
}
