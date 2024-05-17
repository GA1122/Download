static struct ctl_table *is_branch_in(struct ctl_table *branch,
				      struct ctl_table *table)
{
	struct ctl_table *p;
	const char *s = branch->procname;

	 
	if (!s || !branch->child)
		return NULL;

	 
	if (branch[1].procname)
		return NULL;

	 
	for (p = table; p->procname; p++) {
		if (!p->child)
			continue;
		if (p->procname && strcmp(p->procname, s) == 0)
			return p;
	}
	return NULL;
}
