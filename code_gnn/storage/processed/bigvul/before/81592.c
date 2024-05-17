static bool is_or(struct prog_entry *prog, int i)
{
	int target;

	 
	target = prog[i].target + 1;
	 
	if (prog[target].pred)
		return false;

	 
	return prog[i].when_to_branch == prog[target].target;
}
