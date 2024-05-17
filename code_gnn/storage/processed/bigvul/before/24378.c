int jbd2_journal_file_inode(handle_t *handle, struct jbd2_inode *jinode)
{
	transaction_t *transaction = handle->h_transaction;
	journal_t *journal = transaction->t_journal;

	if (is_handle_aborted(handle))
		return -EIO;

	jbd_debug(4, "Adding inode %lu, tid:%d\n", jinode->i_vfs_inode->i_ino,
			transaction->t_tid);

	 
	if (jinode->i_transaction == transaction ||
	    jinode->i_next_transaction == transaction)
		return 0;

	spin_lock(&journal->j_list_lock);

	if (jinode->i_transaction == transaction ||
	    jinode->i_next_transaction == transaction)
		goto done;

	 
	if (!transaction->t_need_data_flush)
		transaction->t_need_data_flush = 1;
	 
	if (jinode->i_transaction) {
		J_ASSERT(jinode->i_next_transaction == NULL);
		J_ASSERT(jinode->i_transaction ==
					journal->j_committing_transaction);
		jinode->i_next_transaction = transaction;
		goto done;
	}
	 
	J_ASSERT(!jinode->i_next_transaction);
	jinode->i_transaction = transaction;
	list_add(&jinode->i_list, &transaction->t_inode_list);
done:
	spin_unlock(&journal->j_list_lock);

	return 0;
}
