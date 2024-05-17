trace_insert_eval_map_file(struct module *mod, struct trace_eval_map **start,
			   int len)
{
	struct trace_eval_map **stop;
	struct trace_eval_map **map;
	union trace_eval_map_item *map_array;
	union trace_eval_map_item *ptr;

	stop = start + len;

	 
	map_array = kmalloc_array(len + 2, sizeof(*map_array), GFP_KERNEL);
	if (!map_array) {
		pr_warn("Unable to allocate trace eval mapping\n");
		return;
	}

	mutex_lock(&trace_eval_mutex);

	if (!trace_eval_maps)
		trace_eval_maps = map_array;
	else {
		ptr = trace_eval_maps;
		for (;;) {
			ptr = trace_eval_jmp_to_tail(ptr);
			if (!ptr->tail.next)
				break;
			ptr = ptr->tail.next;

		}
		ptr->tail.next = map_array;
	}
	map_array->head.mod = mod;
	map_array->head.length = len;
	map_array++;

	for (map = start; (unsigned long)map < (unsigned long)stop; map++) {
		map_array->map = **map;
		map_array++;
	}
	memset(map_array, 0, sizeof(*map_array));

	mutex_unlock(&trace_eval_mutex);
}
