static inline int calculate_order(int size)
{
	int order;
	int min_objects;
	int fraction;

	 
	min_objects = slub_min_objects;
	if (!min_objects)
		min_objects = 4 * (fls(nr_cpu_ids) + 1);
	while (min_objects > 1) {
		fraction = 16;
		while (fraction >= 4) {
			order = slab_order(size, min_objects,
						slub_max_order, fraction);
			if (order <= slub_max_order)
				return order;
			fraction /= 2;
		}
		min_objects /= 2;
	}

	 
	order = slab_order(size, 1, slub_max_order, 1);
	if (order <= slub_max_order)
		return order;

	 
	order = slab_order(size, 1, MAX_ORDER, 1);
	if (order <= MAX_ORDER)
		return order;
	return -ENOSYS;
}
