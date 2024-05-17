assoc_array_walk(const struct assoc_array *array,
		 const struct assoc_array_ops *ops,
		 const void *index_key,
		 struct assoc_array_walk_result *result)
{
	struct assoc_array_shortcut *shortcut;
	struct assoc_array_node *node;
	struct assoc_array_ptr *cursor, *ptr;
	unsigned long sc_segments, dissimilarity;
	unsigned long segments;
	int level, sc_level, next_sc_level;
	int slot;

	pr_devel("-->%s()\n", __func__);

	cursor = ACCESS_ONCE(array->root);
	if (!cursor)
		return assoc_array_walk_tree_empty;

	level = 0;

	 
jumped:
	segments = ops->get_key_chunk(index_key, level);
	pr_devel("segments[%d]: %lx\n", level, segments);

	if (assoc_array_ptr_is_shortcut(cursor))
		goto follow_shortcut;

consider_node:
	node = assoc_array_ptr_to_node(cursor);
	smp_read_barrier_depends();

	slot = segments >> (level & ASSOC_ARRAY_KEY_CHUNK_MASK);
	slot &= ASSOC_ARRAY_FAN_MASK;
	ptr = ACCESS_ONCE(node->slots[slot]);

	pr_devel("consider slot %x [ix=%d type=%lu]\n",
		 slot, level, (unsigned long)ptr & 3);

	if (!assoc_array_ptr_is_meta(ptr)) {
		 
		result->terminal_node.node = node;
		result->terminal_node.level = level;
		result->terminal_node.slot = slot;
		pr_devel("<--%s() = terminal_node\n", __func__);
		return assoc_array_walk_found_terminal_node;
	}

	if (assoc_array_ptr_is_node(ptr)) {
		 
		cursor = ptr;
		level += ASSOC_ARRAY_LEVEL_STEP;
		if ((level & ASSOC_ARRAY_KEY_CHUNK_MASK) != 0)
			goto consider_node;
		goto jumped;
	}

	 
	cursor = ptr;
follow_shortcut:
	shortcut = assoc_array_ptr_to_shortcut(cursor);
	smp_read_barrier_depends();
	pr_devel("shortcut to %d\n", shortcut->skip_to_level);
	sc_level = level + ASSOC_ARRAY_LEVEL_STEP;
	BUG_ON(sc_level > shortcut->skip_to_level);

	do {
		 
		if ((sc_level & ASSOC_ARRAY_KEY_CHUNK_MASK) == 0)
			segments = ops->get_key_chunk(index_key, sc_level);

		sc_segments = shortcut->index_key[sc_level >> ASSOC_ARRAY_KEY_CHUNK_SHIFT];
		dissimilarity = segments ^ sc_segments;

		if (round_up(sc_level, ASSOC_ARRAY_KEY_CHUNK_SIZE) > shortcut->skip_to_level) {
			 
			int shift = shortcut->skip_to_level & ASSOC_ARRAY_KEY_CHUNK_MASK;
			dissimilarity &= ~(ULONG_MAX << shift);
			next_sc_level = shortcut->skip_to_level;
		} else {
			next_sc_level = sc_level + ASSOC_ARRAY_KEY_CHUNK_SIZE;
			next_sc_level = round_down(next_sc_level, ASSOC_ARRAY_KEY_CHUNK_SIZE);
		}

		if (dissimilarity != 0) {
			 
			result->wrong_shortcut.shortcut = shortcut;
			result->wrong_shortcut.level = level;
			result->wrong_shortcut.sc_level = sc_level;
			result->wrong_shortcut.sc_segments = sc_segments;
			result->wrong_shortcut.dissimilarity = dissimilarity;
			return assoc_array_walk_found_wrong_shortcut;
		}

		sc_level = next_sc_level;
	} while (sc_level < shortcut->skip_to_level);

	 
	cursor = ACCESS_ONCE(shortcut->next_node);
	if (((level ^ sc_level) & ~ASSOC_ARRAY_KEY_CHUNK_MASK) != 0) {
		level = sc_level;
		goto jumped;
	} else {
		level = sc_level;
		goto consider_node;
	}
}
