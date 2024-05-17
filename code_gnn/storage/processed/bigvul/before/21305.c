int do_migrate_pages(struct mm_struct *mm,
	const nodemask_t *from_nodes, const nodemask_t *to_nodes, int flags)
{
	int busy = 0;
	int err;
	nodemask_t tmp;

	err = migrate_prep();
	if (err)
		return err;

	down_read(&mm->mmap_sem);

	err = migrate_vmas(mm, from_nodes, to_nodes, flags);
	if (err)
		goto out;

	 

	tmp = *from_nodes;
	while (!nodes_empty(tmp)) {
		int s,d;
		int source = -1;
		int dest = 0;

		for_each_node_mask(s, tmp) {
			d = node_remap(s, *from_nodes, *to_nodes);
			if (s == d)
				continue;

			source = s;	 
			dest = d;

			 
			if (!node_isset(dest, tmp))
				break;
		}
		if (source == -1)
			break;

		node_clear(source, tmp);
		err = migrate_to_node(mm, source, dest, flags);
		if (err > 0)
			busy += err;
		if (err < 0)
			break;
	}
out:
	up_read(&mm->mmap_sem);
	if (err < 0)
		return err;
	return busy;

}
