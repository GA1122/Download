static int add_parents_to_list(struct rev_info *revs, struct commit *commit,
		    struct commit_list **list, struct commit_list **cache_ptr)
{
	struct commit_list *parent = commit->parents;
	unsigned left_flag;
	struct commit_list *cached_base = cache_ptr ? *cache_ptr : NULL;

	if (commit->object.flags & ADDED)
		return 0;
	commit->object.flags |= ADDED;

	if (revs->include_check &&
	    !revs->include_check(commit, revs->include_check_data))
		return 0;

	 
	if (commit->object.flags & UNINTERESTING) {
		while (parent) {
			struct commit *p = parent->item;
			parent = parent->next;
			if (p)
				p->object.flags |= UNINTERESTING;
			if (parse_commit_gently(p, 1) < 0)
				continue;
			if (p->parents)
				mark_parents_uninteresting(p);
			if (p->object.flags & SEEN)
				continue;
			p->object.flags |= SEEN;
			commit_list_insert_by_date_cached(p, list, cached_base, cache_ptr);
		}
		return 0;
	}

	 
	try_to_simplify_commit(revs, commit);

	if (revs->no_walk)
		return 0;

	left_flag = (commit->object.flags & SYMMETRIC_LEFT);

	for (parent = commit->parents; parent; parent = parent->next) {
		struct commit *p = parent->item;

		if (parse_commit_gently(p, revs->ignore_missing_links) < 0)
			return -1;
		if (revs->show_source && !p->util)
			p->util = commit->util;
		p->object.flags |= left_flag;
		if (!(p->object.flags & SEEN)) {
			p->object.flags |= SEEN;
			commit_list_insert_by_date_cached(p, list, cached_base, cache_ptr);
		}
		if (revs->first_parent_only)
			break;
	}
	return 0;
}
