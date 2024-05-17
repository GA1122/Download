static bool assoc_array_insert_into_terminal_node(struct assoc_array_edit *edit,
						  const struct assoc_array_ops *ops,
						  const void *index_key,
						  struct assoc_array_walk_result *result)
{
	struct assoc_array_shortcut *shortcut, *new_s0;
	struct assoc_array_node *node, *new_n0, *new_n1, *side;
	struct assoc_array_ptr *ptr;
	unsigned long dissimilarity, base_seg, blank;
	size_t keylen;
	bool have_meta;
	int level, diff;
	int slot, next_slot, free_slot, i, j;

	node	= result->terminal_node.node;
	level	= result->terminal_node.level;
	edit->segment_cache[ASSOC_ARRAY_FAN_OUT] = result->terminal_node.slot;

	pr_devel("-->%s()\n", __func__);

	 
	free_slot = -1;

	 
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
		ptr = node->slots[i];
		if (!ptr) {
			free_slot = i;
			continue;
		}
		if (ops->compare_object(assoc_array_ptr_to_leaf(ptr), index_key)) {
			pr_devel("replace in slot %d\n", i);
			edit->leaf_p = &node->slots[i];
			edit->dead_leaf = node->slots[i];
			pr_devel("<--%s() = ok [replace]\n", __func__);
			return true;
		}
	}

	 
	if (free_slot >= 0) {
		pr_devel("insert in free slot %d\n", free_slot);
		edit->leaf_p = &node->slots[free_slot];
		edit->adjust_count_on = node;
		pr_devel("<--%s() = ok [insert]\n", __func__);
		return true;
	}

	 
	new_n0 = kzalloc(sizeof(struct assoc_array_node), GFP_KERNEL);
	if (!new_n0)
		return false;
	edit->new_meta[0] = assoc_array_node_to_ptr(new_n0);
	new_n1 = kzalloc(sizeof(struct assoc_array_node), GFP_KERNEL);
	if (!new_n1)
		return false;
	edit->new_meta[1] = assoc_array_node_to_ptr(new_n1);

	 
	pr_devel("no spare slots\n");
	have_meta = false;
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
		ptr = node->slots[i];
		if (assoc_array_ptr_is_meta(ptr)) {
			edit->segment_cache[i] = 0xff;
			have_meta = true;
			continue;
		}
		base_seg = ops->get_object_key_chunk(
			assoc_array_ptr_to_leaf(ptr), level);
		base_seg >>= level & ASSOC_ARRAY_KEY_CHUNK_MASK;
		edit->segment_cache[i] = base_seg & ASSOC_ARRAY_FAN_MASK;
	}

	if (have_meta) {
		pr_devel("have meta\n");
		goto split_node;
	}

	 
	dissimilarity = 0;
	base_seg = edit->segment_cache[0];
	for (i = 1; i < ASSOC_ARRAY_FAN_OUT; i++)
		dissimilarity |= edit->segment_cache[i] ^ base_seg;

	pr_devel("only leaves; dissimilarity=%lx\n", dissimilarity);

	if ((dissimilarity & ASSOC_ARRAY_FAN_MASK) == 0) {
		 
		if ((edit->segment_cache[ASSOC_ARRAY_FAN_OUT] ^ base_seg) == 0)
			goto all_leaves_cluster_together;

		 
		goto present_leaves_cluster_but_not_new_leaf;
	}

split_node:
	pr_devel("split node\n");

	 
	edit->set[0].to = assoc_array_node_to_ptr(new_n0);
	new_n0->back_pointer = node->back_pointer;
	new_n0->parent_slot = node->parent_slot;
	new_n1->back_pointer = assoc_array_node_to_ptr(new_n0);
	new_n1->parent_slot = -1;  

do_split_node:
	pr_devel("do_split_node\n");

	new_n0->nr_leaves_on_branch = node->nr_leaves_on_branch;
	new_n1->nr_leaves_on_branch = 0;

	 
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
		slot = edit->segment_cache[i];
		if (slot != 0xff)
			for (j = i + 1; j < ASSOC_ARRAY_FAN_OUT + 1; j++)
				if (edit->segment_cache[j] == slot)
					goto found_slot_for_multiple_occupancy;
	}
found_slot_for_multiple_occupancy:
	pr_devel("same slot: %x %x [%02x]\n", i, j, slot);
	BUG_ON(i >= ASSOC_ARRAY_FAN_OUT);
	BUG_ON(j >= ASSOC_ARRAY_FAN_OUT + 1);
	BUG_ON(slot >= ASSOC_ARRAY_FAN_OUT);

	new_n1->parent_slot = slot;

	 
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++)
		if (assoc_array_ptr_is_meta(node->slots[i]))
			new_n0->slots[i] = node->slots[i];
		else
			new_n0->slots[i] = NULL;
	BUG_ON(new_n0->slots[slot] != NULL);
	new_n0->slots[slot] = assoc_array_node_to_ptr(new_n1);

	 
	free_slot = -1;
	next_slot = 0;
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
		if (assoc_array_ptr_is_meta(node->slots[i]))
			continue;
		if (edit->segment_cache[i] == slot) {
			new_n1->slots[next_slot++] = node->slots[i];
			new_n1->nr_leaves_on_branch++;
		} else {
			do {
				free_slot++;
			} while (new_n0->slots[free_slot] != NULL);
			new_n0->slots[free_slot] = node->slots[i];
		}
	}

	pr_devel("filtered: f=%x n=%x\n", free_slot, next_slot);

	if (edit->segment_cache[ASSOC_ARRAY_FAN_OUT] != slot) {
		do {
			free_slot++;
		} while (new_n0->slots[free_slot] != NULL);
		edit->leaf_p = &new_n0->slots[free_slot];
		edit->adjust_count_on = new_n0;
	} else {
		edit->leaf_p = &new_n1->slots[next_slot++];
		edit->adjust_count_on = new_n1;
	}

	BUG_ON(next_slot <= 1);

	edit->set_backpointers_to = assoc_array_node_to_ptr(new_n0);
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
		if (edit->segment_cache[i] == 0xff) {
			ptr = node->slots[i];
			BUG_ON(assoc_array_ptr_is_leaf(ptr));
			if (assoc_array_ptr_is_node(ptr)) {
				side = assoc_array_ptr_to_node(ptr);
				edit->set_backpointers[i] = &side->back_pointer;
			} else {
				shortcut = assoc_array_ptr_to_shortcut(ptr);
				edit->set_backpointers[i] = &shortcut->back_pointer;
			}
		}
	}

	ptr = node->back_pointer;
	if (!ptr)
		edit->set[0].ptr = &edit->array->root;
	else if (assoc_array_ptr_is_node(ptr))
		edit->set[0].ptr = &assoc_array_ptr_to_node(ptr)->slots[node->parent_slot];
	else
		edit->set[0].ptr = &assoc_array_ptr_to_shortcut(ptr)->next_node;
	edit->excised_meta[0] = assoc_array_node_to_ptr(node);
	pr_devel("<--%s() = ok [split node]\n", __func__);
	return true;

present_leaves_cluster_but_not_new_leaf:
	 
	pr_devel("present leaves cluster but not new leaf\n");

	new_n0->back_pointer = node->back_pointer;
	new_n0->parent_slot = node->parent_slot;
	new_n0->nr_leaves_on_branch = node->nr_leaves_on_branch;
	new_n1->back_pointer = assoc_array_node_to_ptr(new_n0);
	new_n1->parent_slot = edit->segment_cache[0];
	new_n1->nr_leaves_on_branch = node->nr_leaves_on_branch;
	edit->adjust_count_on = new_n0;

	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++)
		new_n1->slots[i] = node->slots[i];

	new_n0->slots[edit->segment_cache[0]] = assoc_array_node_to_ptr(new_n0);
	edit->leaf_p = &new_n0->slots[edit->segment_cache[ASSOC_ARRAY_FAN_OUT]];

	edit->set[0].ptr = &assoc_array_ptr_to_node(node->back_pointer)->slots[node->parent_slot];
	edit->set[0].to = assoc_array_node_to_ptr(new_n0);
	edit->excised_meta[0] = assoc_array_node_to_ptr(node);
	pr_devel("<--%s() = ok [insert node before]\n", __func__);
	return true;

all_leaves_cluster_together:
	 
	pr_devel("all leaves cluster together\n");
	diff = INT_MAX;
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
		int x = ops->diff_objects(assoc_array_ptr_to_leaf(node->slots[i]),
					  index_key);
		if (x < diff) {
			BUG_ON(x < 0);
			diff = x;
		}
	}
	BUG_ON(diff == INT_MAX);
	BUG_ON(diff < level + ASSOC_ARRAY_LEVEL_STEP);

	keylen = round_up(diff, ASSOC_ARRAY_KEY_CHUNK_SIZE);
	keylen >>= ASSOC_ARRAY_KEY_CHUNK_SHIFT;

	new_s0 = kzalloc(sizeof(struct assoc_array_shortcut) +
			 keylen * sizeof(unsigned long), GFP_KERNEL);
	if (!new_s0)
		return false;
	edit->new_meta[2] = assoc_array_shortcut_to_ptr(new_s0);

	edit->set[0].to = assoc_array_shortcut_to_ptr(new_s0);
	new_s0->back_pointer = node->back_pointer;
	new_s0->parent_slot = node->parent_slot;
	new_s0->next_node = assoc_array_node_to_ptr(new_n0);
	new_n0->back_pointer = assoc_array_shortcut_to_ptr(new_s0);
	new_n0->parent_slot = 0;
	new_n1->back_pointer = assoc_array_node_to_ptr(new_n0);
	new_n1->parent_slot = -1;  

	new_s0->skip_to_level = level = diff & ~ASSOC_ARRAY_LEVEL_STEP_MASK;
	pr_devel("skip_to_level = %d [diff %d]\n", level, diff);
	BUG_ON(level <= 0);

	for (i = 0; i < keylen; i++)
		new_s0->index_key[i] =
			ops->get_key_chunk(index_key, i * ASSOC_ARRAY_KEY_CHUNK_SIZE);

	blank = ULONG_MAX << (level & ASSOC_ARRAY_KEY_CHUNK_MASK);
	pr_devel("blank off [%zu] %d: %lx\n", keylen - 1, level, blank);
	new_s0->index_key[keylen - 1] &= ~blank;

	 
	for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
		ptr = node->slots[i];
		base_seg = ops->get_object_key_chunk(assoc_array_ptr_to_leaf(ptr),
						     level);
		base_seg >>= level & ASSOC_ARRAY_KEY_CHUNK_MASK;
		edit->segment_cache[i] = base_seg & ASSOC_ARRAY_FAN_MASK;
	}

	base_seg = ops->get_key_chunk(index_key, level);
	base_seg >>= level & ASSOC_ARRAY_KEY_CHUNK_MASK;
	edit->segment_cache[ASSOC_ARRAY_FAN_OUT] = base_seg & ASSOC_ARRAY_FAN_MASK;
	goto do_split_node;
}