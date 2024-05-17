struct kmem_cache *kmem_cache_create(const char *name, size_t size,
		size_t align, unsigned long flags,
		void (*ctor)(struct kmem_cache *, void *))
{
	struct kmem_cache *s;

	down_write(&slub_lock);
	s = find_mergeable(size, align, flags, name, ctor);
	if (s) {
		int cpu;

		s->refcount++;
		 
		s->objsize = max(s->objsize, (int)size);

		 
		for_each_online_cpu(cpu)
			get_cpu_slab(s, cpu)->objsize = s->objsize;

		s->inuse = max_t(int, s->inuse, ALIGN(size, sizeof(void *)));
		up_write(&slub_lock);

		if (sysfs_slab_alias(s, name))
			goto err;
		return s;
	}

	s = kmalloc(kmem_size, GFP_KERNEL);
	if (s) {
		if (kmem_cache_open(s, GFP_KERNEL, name,
				size, align, flags, ctor)) {
			list_add(&s->list, &slab_caches);
			up_write(&slub_lock);
			if (sysfs_slab_add(s))
				goto err;
			return s;
		}
		kfree(s);
	}
	up_write(&slub_lock);

err:
	if (flags & SLAB_PANIC)
		panic("Cannot create slabcache %s\n", name);
	else
		s = NULL;
	return s;
}
