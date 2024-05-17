static void cherry_pick_list(struct commit_list *list, struct rev_info *revs)
{
	struct commit_list *p;
	int left_count = 0, right_count = 0;
	int left_first;
	struct patch_ids ids;
	unsigned cherry_flag;

	 
	for (p = list; p; p = p->next) {
		struct commit *commit = p->item;
		unsigned flags = commit->object.flags;
		if (flags & BOUNDARY)
			;
		else if (flags & SYMMETRIC_LEFT)
			left_count++;
		else
			right_count++;
	}

	if (!left_count || !right_count)
		return;

	left_first = left_count < right_count;
	init_patch_ids(&ids);
	ids.diffopts.pathspec = revs->diffopt.pathspec;

	 
	for (p = list; p; p = p->next) {
		struct commit *commit = p->item;
		unsigned flags = commit->object.flags;

		if (flags & BOUNDARY)
			continue;
		 
		if (left_first != !!(flags & SYMMETRIC_LEFT))
			continue;
		commit->util = add_commit_patch_id(commit, &ids);
	}

	 
	cherry_flag = revs->cherry_mark ? PATCHSAME : SHOWN;

	 
	for (p = list; p; p = p->next) {
		struct commit *commit = p->item;
		struct patch_id *id;
		unsigned flags = commit->object.flags;

		if (flags & BOUNDARY)
			continue;
		 
		if (left_first == !!(flags & SYMMETRIC_LEFT))
			continue;

		 
		id = has_commit_patch_id(commit, &ids);
		if (!id)
			continue;
		id->seen = 1;
		commit->object.flags |= cherry_flag;
	}

	 
	for (p = list; p; p = p->next) {
		struct commit *commit = p->item;
		struct patch_id *ent;

		ent = commit->util;
		if (!ent)
			continue;
		if (ent->seen)
			commit->object.flags |= cherry_flag;
		commit->util = NULL;
	}

	free_patch_ids(&ids);
}
