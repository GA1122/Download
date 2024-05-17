struct ctl_table_header *__register_sysctl_paths(
	struct ctl_table_root *root,
	struct nsproxy *namespaces,
	const struct ctl_path *path, struct ctl_table *table)
{
	struct ctl_table_header *header;
	struct ctl_table *new, **prevp;
	unsigned int n, npath;
	struct ctl_table_set *set;

	 
	for (npath = 0; path[npath].procname; ++npath)
		;

	 
	header = kzalloc(sizeof(struct ctl_table_header) +
			 (2 * npath * sizeof(struct ctl_table)), GFP_KERNEL);
	if (!header)
		return NULL;

	new = (struct ctl_table *) (header + 1);

	 
	prevp = &header->ctl_table;
	for (n = 0; n < npath; ++n, ++path) {
		 
		new->procname = path->procname;
		new->mode     = 0555;

		*prevp = new;
		prevp = &new->child;

		new += 2;
	}
	*prevp = table;
	header->ctl_table_arg = table;

	INIT_LIST_HEAD(&header->ctl_entry);
	header->used = 0;
	header->unregistering = NULL;
	header->root = root;
	sysctl_set_parent(NULL, header->ctl_table);
	header->count = 1;
#ifdef CONFIG_SYSCTL_SYSCALL_CHECK
	if (sysctl_check_table(namespaces, header->ctl_table)) {
		kfree(header);
		return NULL;
	}
#endif
	spin_lock(&sysctl_lock);
	header->set = lookup_header_set(root, namespaces);
	header->attached_by = header->ctl_table;
	header->attached_to = root_table;
	header->parent = &root_table_header;
	for (set = header->set; set; set = set->parent) {
		struct ctl_table_header *p;
		list_for_each_entry(p, &set->list, ctl_entry) {
			if (p->unregistering)
				continue;
			try_attach(p, header);
		}
	}
	header->parent->count++;
	list_add_tail(&header->ctl_entry, &header->set->list);
	spin_unlock(&sysctl_lock);

	return header;
}
