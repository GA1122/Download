struct assoc_array_edit *assoc_array_insert(struct assoc_array *array,
					    const struct assoc_array_ops *ops,
					    const void *index_key,
					    void *object)
{
	struct assoc_array_walk_result result;
	struct assoc_array_edit *edit;

	pr_devel("-->%s()\n", __func__);

	 
	BUG_ON(assoc_array_ptr_is_meta(object));

	edit = kzalloc(sizeof(struct assoc_array_edit), GFP_KERNEL);
	if (!edit)
		return ERR_PTR(-ENOMEM);
	edit->array = array;
	edit->ops = ops;
	edit->leaf = assoc_array_leaf_to_ptr(object);
	edit->adjust_count_by = 1;

	switch (assoc_array_walk(array, ops, index_key, &result)) {
	case assoc_array_walk_tree_empty:
		 
		if (!assoc_array_insert_in_empty_tree(edit))
			goto enomem;
		return edit;

	case assoc_array_walk_found_terminal_node:
		 
		if (!assoc_array_insert_into_terminal_node(edit, ops, index_key,
							   &result))
			goto enomem;
		return edit;

	case assoc_array_walk_found_wrong_shortcut:
		 
		if (!assoc_array_insert_mid_shortcut(edit, ops, &result))
			goto enomem;
		return edit;
	}

enomem:
	 
	pr_devel("enomem\n");
	assoc_array_cancel_edit(edit);
	return ERR_PTR(-ENOMEM);
}
