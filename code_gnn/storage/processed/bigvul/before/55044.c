static int still_interesting(struct commit_list *src, unsigned long date, int slop,
			     struct commit **interesting_cache)
{
	 
	if (!src)
		return 0;

	 
	if (date <= src->item->date)
		return SLOP;

	 
	if (!everybody_uninteresting(src, interesting_cache))
		return SLOP;

	 
	return slop-1;
}
