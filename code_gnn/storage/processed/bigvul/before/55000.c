static struct commit *handle_commit(struct rev_info *revs,
				    struct object_array_entry *entry)
{
	struct object *object = entry->item;
	const char *name = entry->name;
	const char *path = entry->path;
	unsigned int mode = entry->mode;
	unsigned long flags = object->flags;

	 
	while (object->type == OBJ_TAG) {
		struct tag *tag = (struct tag *) object;
		if (revs->tag_objects && !(flags & UNINTERESTING))
			add_pending_object(revs, object, tag->tag);
		if (!tag->tagged)
			die("bad tag");
		object = parse_object(tag->tagged->oid.hash);
		if (!object) {
			if (flags & UNINTERESTING)
				return NULL;
			die("bad object %s", oid_to_hex(&tag->tagged->oid));
		}
		object->flags |= flags;
		 
		path = NULL;
		mode = 0;
	}

	 
	if (object->type == OBJ_COMMIT) {
		struct commit *commit = (struct commit *)object;
		if (parse_commit(commit) < 0)
			die("unable to parse commit %s", name);
		if (flags & UNINTERESTING) {
			mark_parents_uninteresting(commit);
			revs->limited = 1;
		}
		if (revs->show_source && !commit->util)
			commit->util = xstrdup(name);
		return commit;
	}

	 
	if (object->type == OBJ_TREE) {
		struct tree *tree = (struct tree *)object;
		if (!revs->tree_objects)
			return NULL;
		if (flags & UNINTERESTING) {
			mark_tree_contents_uninteresting(tree);
			return NULL;
		}
		add_pending_object_with_path(revs, object, name, mode, path);
		return NULL;
	}

	 
	if (object->type == OBJ_BLOB) {
		if (!revs->blob_objects)
			return NULL;
		if (flags & UNINTERESTING)
			return NULL;
		add_pending_object_with_path(revs, object, name, mode, path);
		return NULL;
	}
	die("%s is unknown object", name);
}
