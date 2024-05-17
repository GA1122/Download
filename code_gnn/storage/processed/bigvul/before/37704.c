static int assoc_array_subtree_iterate(const struct assoc_array_ptr *root,
				       const struct assoc_array_ptr *stop,
				       int (*iterator)(const void *leaf,
						       void *iterator_data),
				       void *iterator_data)
{
	const struct assoc_array_shortcut *shortcut;
	const struct assoc_array_node *node;
	const struct assoc_array_ptr *cursor, *ptr, *parent;
	unsigned long has_meta;
	int slot, ret;

	cursor = root;

begin_node:
	if (assoc_array_ptr_is_shortcut(cursor)) {
		 
		shortcut = assoc_array_ptr_to_shortcut(cursor);
		smp_read_barrier_depends();
		cursor = ACCESS_ONCE(shortcut->next_node);
	}

	node = assoc_array_ptr_to_node(cursor);
	smp_read_barrier_depends();
	slot = 0;

	 
	has_meta = 0;
	for (; slot < ASSOC_ARRAY_FAN_OUT; slot++) {
		ptr = ACCESS_ONCE(node->slots[slot]);
		has_meta |= (unsigned long)ptr;
		if (ptr && assoc_array_ptr_is_leaf(ptr)) {
			 
			smp_read_barrier_depends();

			 
			ret = iterator(assoc_array_ptr_to_leaf(ptr),
				       iterator_data);
			if (ret)
				return ret;
		}
	}

	 
	if (!(has_meta & ASSOC_ARRAY_PTR_META_TYPE))
		goto finished_node;
	slot = 0;

continue_node:
	node = assoc_array_ptr_to_node(cursor);
	smp_read_barrier_depends();

	for (; slot < ASSOC_ARRAY_FAN_OUT; slot++) {
		ptr = ACCESS_ONCE(node->slots[slot]);
		if (assoc_array_ptr_is_meta(ptr)) {
			cursor = ptr;
			goto begin_node;
		}
	}

finished_node:
	 
	parent = ACCESS_ONCE(node->back_pointer);
	slot = node->parent_slot;
	if (parent == stop)
		return 0;

	if (assoc_array_ptr_is_shortcut(parent)) {
		shortcut = assoc_array_ptr_to_shortcut(parent);
		smp_read_barrier_depends();
		cursor = parent;
		parent = ACCESS_ONCE(shortcut->back_pointer);
		slot = shortcut->parent_slot;
		if (parent == stop)
			return 0;
	}

	 
	cursor = parent;
	slot++;
	goto continue_node;
}
