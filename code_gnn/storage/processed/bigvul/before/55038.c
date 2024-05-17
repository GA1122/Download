static void save_parents(struct rev_info *revs, struct commit *commit)
{
	struct commit_list **pp;

	if (!revs->saved_parents_slab) {
		revs->saved_parents_slab = xmalloc(sizeof(struct saved_parents));
		init_saved_parents(revs->saved_parents_slab);
	}

	pp = saved_parents_at(revs->saved_parents_slab, commit);

	 
	if (*pp)
		return;
	if (commit->parents)
		*pp = copy_commit_list(commit->parents);
	else
		*pp = EMPTY_PARENT_LIST;
}
