void mark_parents_uninteresting(struct commit *commit)
{
	struct commit_list *parents = NULL, *l;

	for (l = commit->parents; l; l = l->next)
		commit_list_insert(l->item, &parents);

	while (parents) {
		struct commit *commit = parents->item;
		l = parents;
		parents = parents->next;
		free(l);

		while (commit) {
			 
			if (!has_sha1_file(commit->object.sha1))
				commit->object.parsed = 1;

			if (commit->object.flags & UNINTERESTING)
				break;

			commit->object.flags |= UNINTERESTING;

			 
			if (!commit->parents)
				break;

			for (l = commit->parents->next; l; l = l->next)
				commit_list_insert(l->item, &parents);
			commit = commit->parents->item;
		}
	}
}
