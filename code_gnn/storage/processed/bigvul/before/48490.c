static int register_leaf_sysctl_tables(const char *path, char *pos,
	struct ctl_table_header ***subheader, struct ctl_table_set *set,
	struct ctl_table *table)
{
	struct ctl_table *ctl_table_arg = NULL;
	struct ctl_table *entry, *files;
	int nr_files = 0;
	int nr_dirs = 0;
	int err = -ENOMEM;

	for (entry = table; entry->procname; entry++) {
		if (entry->child)
			nr_dirs++;
		else
			nr_files++;
	}

	files = table;
	 
	if (nr_dirs && nr_files) {
		struct ctl_table *new;
		files = kzalloc(sizeof(struct ctl_table) * (nr_files + 1),
				GFP_KERNEL);
		if (!files)
			goto out;

		ctl_table_arg = files;
		for (new = files, entry = table; entry->procname; entry++) {
			if (entry->child)
				continue;
			*new = *entry;
			new++;
		}
	}

	 
	if (nr_files || !nr_dirs) {
		struct ctl_table_header *header;
		header = __register_sysctl_table(set, path, files);
		if (!header) {
			kfree(ctl_table_arg);
			goto out;
		}

		 
		header->ctl_table_arg = ctl_table_arg;
		**subheader = header;
		(*subheader)++;
	}

	 
	for (entry = table; entry->procname; entry++) {
		char *child_pos;

		if (!entry->child)
			continue;

		err = -ENAMETOOLONG;
		child_pos = append_path(path, pos, entry->procname);
		if (!child_pos)
			goto out;

		err = register_leaf_sysctl_tables(path, child_pos, subheader,
						  set, entry->child);
		pos[0] = '\0';
		if (err)
			goto out;
	}
	err = 0;
out:
	 
	return err;
}