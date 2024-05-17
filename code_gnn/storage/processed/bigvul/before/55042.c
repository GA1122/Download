static void simplify_merges(struct rev_info *revs)
{
	struct commit_list *list, *next;
	struct commit_list *yet_to_do, **tail;
	struct commit *commit;

	if (!revs->prune)
		return;

	 
	yet_to_do = NULL;
	for (list = revs->commits; list; list = next) {
		commit = list->item;
		next = list->next;
		 
		commit_list_insert(commit, &yet_to_do);
	}
	while (yet_to_do) {
		list = yet_to_do;
		yet_to_do = NULL;
		tail = &yet_to_do;
		while (list) {
			commit = pop_commit(&list);
			tail = simplify_one(revs, commit, tail);
		}
	}

	 
	list = revs->commits;
	revs->commits = NULL;
	tail = &revs->commits;
	while (list) {
		struct merge_simplify_state *st;

		commit = pop_commit(&list);
		st = locate_simplify_state(revs, commit);
		if (st->simplified == commit)
			tail = &commit_list_insert(commit, tail)->next;
	}
}
