int jbd2_journal_dirty_metadata(handle_t *handle, struct buffer_head *bh)
{
	transaction_t *transaction = handle->h_transaction;
	journal_t *journal = transaction->t_journal;
	struct journal_head *jh = bh2jh(bh);
	int ret = 0;

	jbd_debug(5, "journal_head %p\n", jh);
	JBUFFER_TRACE(jh, "entry");
	if (is_handle_aborted(handle))
		goto out;
	if (!buffer_jbd(bh)) {
		ret = -EUCLEAN;
		goto out;
	}

	jbd_lock_bh_state(bh);

	if (jh->b_modified == 0) {
		 
		jh->b_modified = 1;
		J_ASSERT_JH(jh, handle->h_buffer_credits > 0);
		handle->h_buffer_credits--;
	}

	 
	if (jh->b_transaction == transaction && jh->b_jlist == BJ_Metadata) {
		JBUFFER_TRACE(jh, "fastpath");
		if (unlikely(jh->b_transaction !=
			     journal->j_running_transaction)) {
			printk(KERN_EMERG "JBD: %s: "
			       "jh->b_transaction (%llu, %p, %u) != "
			       "journal->j_running_transaction (%p, %u)",
			       journal->j_devname,
			       (unsigned long long) bh->b_blocknr,
			       jh->b_transaction,
			       jh->b_transaction ? jh->b_transaction->t_tid : 0,
			       journal->j_running_transaction,
			       journal->j_running_transaction ?
			       journal->j_running_transaction->t_tid : 0);
			ret = -EINVAL;
		}
		goto out_unlock_bh;
	}

	set_buffer_jbddirty(bh);

	 
	if (jh->b_transaction != transaction) {
		JBUFFER_TRACE(jh, "already on other transaction");
		if (unlikely(jh->b_transaction !=
			     journal->j_committing_transaction)) {
			printk(KERN_EMERG "JBD: %s: "
			       "jh->b_transaction (%llu, %p, %u) != "
			       "journal->j_committing_transaction (%p, %u)",
			       journal->j_devname,
			       (unsigned long long) bh->b_blocknr,
			       jh->b_transaction,
			       jh->b_transaction ? jh->b_transaction->t_tid : 0,
			       journal->j_committing_transaction,
			       journal->j_committing_transaction ?
			       journal->j_committing_transaction->t_tid : 0);
			ret = -EINVAL;
		}
		if (unlikely(jh->b_next_transaction != transaction)) {
			printk(KERN_EMERG "JBD: %s: "
			       "jh->b_next_transaction (%llu, %p, %u) != "
			       "transaction (%p, %u)",
			       journal->j_devname,
			       (unsigned long long) bh->b_blocknr,
			       jh->b_next_transaction,
			       jh->b_next_transaction ?
			       jh->b_next_transaction->t_tid : 0,
			       transaction, transaction->t_tid);
			ret = -EINVAL;
		}
		 
		goto out_unlock_bh;
	}

	 
	J_ASSERT_JH(jh, jh->b_frozen_data == NULL);

	JBUFFER_TRACE(jh, "file as BJ_Metadata");
	spin_lock(&journal->j_list_lock);
	__jbd2_journal_file_buffer(jh, handle->h_transaction, BJ_Metadata);
	spin_unlock(&journal->j_list_lock);
out_unlock_bh:
	jbd_unlock_bh_state(bh);
out:
	JBUFFER_TRACE(jh, "exit");
	WARN_ON(ret);	 
	return ret;
}
