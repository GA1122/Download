struct ftrace_rec_iter *ftrace_rec_iter_start(void)
{
	 
	static struct ftrace_rec_iter ftrace_rec_iter;
	struct ftrace_rec_iter *iter = &ftrace_rec_iter;

	iter->pg = ftrace_pages_start;
	iter->index = 0;

	 
	while (iter->pg && !iter->pg->index)
		iter->pg = iter->pg->next;

	if (!iter->pg)
		return NULL;

	return iter;
}
