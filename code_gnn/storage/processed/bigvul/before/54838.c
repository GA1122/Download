static struct object_entry **compute_write_order(void)
{
	unsigned int i, wo_end, last_untagged;

	struct object_entry **wo = xmalloc(to_pack.nr_objects * sizeof(*wo));
	struct object_entry *objects = to_pack.objects;

	for (i = 0; i < to_pack.nr_objects; i++) {
		objects[i].tagged = 0;
		objects[i].filled = 0;
		objects[i].delta_child = NULL;
		objects[i].delta_sibling = NULL;
	}

	 
	for (i = to_pack.nr_objects; i > 0;) {
		struct object_entry *e = &objects[--i];
		if (!e->delta)
			continue;
		 
		e->delta_sibling = e->delta->delta_child;
		e->delta->delta_child = e;
	}

	 
	for_each_tag_ref(mark_tagged, NULL);

	 
	for (i = wo_end = 0; i < to_pack.nr_objects; i++) {
		if (objects[i].tagged)
			break;
		add_to_write_order(wo, &wo_end, &objects[i]);
	}
	last_untagged = i;

	 
	for (; i < to_pack.nr_objects; i++) {
		if (objects[i].tagged)
			add_to_write_order(wo, &wo_end, &objects[i]);
	}

	 
	for (i = last_untagged; i < to_pack.nr_objects; i++) {
		if (objects[i].type != OBJ_COMMIT &&
		    objects[i].type != OBJ_TAG)
			continue;
		add_to_write_order(wo, &wo_end, &objects[i]);
	}

	 
	for (i = last_untagged; i < to_pack.nr_objects; i++) {
		if (objects[i].type != OBJ_TREE)
			continue;
		add_to_write_order(wo, &wo_end, &objects[i]);
	}

	 
	for (i = last_untagged; i < to_pack.nr_objects; i++) {
		if (!objects[i].filled)
			add_family_to_write_order(wo, &wo_end, &objects[i]);
	}

	if (wo_end != to_pack.nr_objects)
		die("ordered %u objects, expected %"PRIu32, wo_end, to_pack.nr_objects);

	return wo;
}
