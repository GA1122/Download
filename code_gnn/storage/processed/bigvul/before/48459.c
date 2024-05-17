static bool get_links(struct ctl_dir *dir,
	struct ctl_table *table, struct ctl_table_root *link_root)
{
	struct ctl_table_header *head;
	struct ctl_table *entry, *link;

	 
	for (entry = table; entry->procname; entry++) {
		const char *procname = entry->procname;
		link = find_entry(&head, dir, procname, strlen(procname));
		if (!link)
			return false;
		if (S_ISDIR(link->mode) && S_ISDIR(entry->mode))
			continue;
		if (S_ISLNK(link->mode) && (link->data == link_root))
			continue;
		return false;
	}

	 
	for (entry = table; entry->procname; entry++) {
		const char *procname = entry->procname;
		link = find_entry(&head, dir, procname, strlen(procname));
		head->nreg++;
	}
	return true;
}
