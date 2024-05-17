static void __free_domain_allocs(struct s_data *d, enum s_alloc what,
				 const struct cpumask *cpu_map)
{
	switch (what) {
	case sa_rootdomain:
		if (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);  
	case sa_sd:
		free_percpu(d->sd);  
	case sa_sd_storage:
		__sdt_free(cpu_map);  
	case sa_none:
		break;
	}
}
