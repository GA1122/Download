static int leave_one_treesame_to_parent(struct rev_info *revs, struct commit *commit)
{
	struct treesame_state *ts = lookup_decoration(&revs->treesame, &commit->object);
	struct commit *unmarked = NULL, *marked = NULL;
	struct commit_list *p;
	unsigned n;

	for (p = commit->parents, n = 0; p; p = p->next, n++) {
		if (ts->treesame[n]) {
			if (p->item->object.flags & TMP_MARK) {
				if (!marked)
					marked = p->item;
			} else {
				if (!unmarked) {
					unmarked = p->item;
					break;
				}
			}
		}
	}

	 
	if (!unmarked && marked) {
		marked->object.flags &= ~TMP_MARK;
		return 1;
	}

	return 0;
}
