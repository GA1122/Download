mlist_free(struct mlist *mlist)
{
	struct mlist *ml;

	if (mlist == NULL)
		return;

	for (ml = mlist->next; ml != mlist;) {
		struct mlist *next = ml->next;
		if (ml->map)
			apprentice_unmap(ml->map);
		free(ml);
		ml = next;
	}
	free(ml);
}