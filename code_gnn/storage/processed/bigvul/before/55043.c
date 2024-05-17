static struct commit_list **simplify_one(struct rev_info *revs, struct commit *commit, struct commit_list **tail)
{
	struct commit_list *p;
	struct commit *parent;
	struct merge_simplify_state *st, *pst;
	int cnt;

	st = locate_simplify_state(revs, commit);

	 
	if (st->simplified)
		return tail;

	 
	if ((commit->object.flags & UNINTERESTING) || !commit->parents) {
		st->simplified = commit;
		return tail;
	}

	 
	for (cnt = 0, p = commit->parents; p; p = p->next) {
		pst = locate_simplify_state(revs, p->item);
		if (!pst->simplified) {
			tail = &commit_list_insert(p->item, tail)->next;
			cnt++;
		}
		if (revs->first_parent_only)
			break;
	}
	if (cnt) {
		tail = &commit_list_insert(commit, tail)->next;
		return tail;
	}

	 
	for (p = commit->parents; p; p = p->next) {
		pst = locate_simplify_state(revs, p->item);
		p->item = pst->simplified;
		if (revs->first_parent_only)
			break;
	}

	if (revs->first_parent_only)
		cnt = 1;
	else
		cnt = remove_duplicate_parents(revs, commit);

	 
	if (1 < cnt) {
		int marked = mark_redundant_parents(revs, commit);
		marked += mark_treesame_root_parents(revs, commit);
		if (marked)
			marked -= leave_one_treesame_to_parent(revs, commit);
		if (marked)
			cnt = remove_marked_parents(revs, commit);
	}

	 
	if (!cnt ||
	    (commit->object.flags & UNINTERESTING) ||
	    !(commit->object.flags & TREESAME) ||
	    (parent = one_relevant_parent(revs, commit->parents)) == NULL)
		st->simplified = commit;
	else {
		pst = locate_simplify_state(revs, parent);
		st->simplified = pst->simplified;
	}
	return tail;
}
