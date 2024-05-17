smb_ofile_lookup_by_fid(
    smb_request_t	*sr,
    uint16_t		fid)
{
	smb_tree_t	*tree = sr->tid_tree;
	smb_llist_t	*of_list;
	smb_ofile_t	*of;

	ASSERT(tree->t_magic == SMB_TREE_MAGIC);

	of_list = &tree->t_ofile_list;

	smb_llist_enter(of_list, RW_READER);
	of = smb_llist_head(of_list);
	while (of) {
		ASSERT(of->f_magic == SMB_OFILE_MAGIC);
		ASSERT(of->f_tree == tree);
		if (of->f_fid == fid)
			break;
		of = smb_llist_next(of_list, of);
	}
	if (of == NULL)
		goto out;

	 
	if (of->f_user != sr->uid_user) {
		of = NULL;
		goto out;
	}

	mutex_enter(&of->f_mutex);
	if (of->f_state != SMB_OFILE_STATE_OPEN) {
		mutex_exit(&of->f_mutex);
		of = NULL;
		goto out;
	}
	of->f_refcnt++;
	mutex_exit(&of->f_mutex);

out:
	smb_llist_exit(of_list);
	return (of);
}