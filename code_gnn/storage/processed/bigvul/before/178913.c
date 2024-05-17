 static int journal_unmap_buffer(journal_t *journal, struct buffer_head *bh)
 {
 	transaction_t *transaction;
 	struct journal_head *jh;
 	int may_free = 1;
 	int ret;
 
 	BUFFER_TRACE(bh, "entry");
 
 	 
 
 	if (!buffer_jbd(bh))
 		goto zap_buffer_unlocked;
 
 	 
 	write_lock(&journal->j_state_lock);
 	jbd_lock_bh_state(bh);
 	spin_lock(&journal->j_list_lock);
 
 	jh = jbd2_journal_grab_journal_head(bh);
 	if (!jh)
 		goto zap_buffer_no_jh;
 
 	 
 	transaction = jh->b_transaction;
 	if (transaction == NULL) {
 		 
 		if (!jh->b_cp_transaction) {
 			JBUFFER_TRACE(jh, "not on any transaction: zap");
 			goto zap_buffer;
 		}
 
 		if (!buffer_dirty(bh)) {
 			 
 			goto zap_buffer;
 		}
 
 		 
 
 		if (journal->j_running_transaction) {
 			 
 			JBUFFER_TRACE(jh, "checkpointed: add to BJ_Forget");
 			ret = __dispose_buffer(jh,
 					journal->j_running_transaction);
 			jbd2_journal_put_journal_head(jh);
 			spin_unlock(&journal->j_list_lock);
 			jbd_unlock_bh_state(bh);
 			write_unlock(&journal->j_state_lock);
 			return ret;
 		} else {
 			 
 			if (journal->j_committing_transaction) {
 				JBUFFER_TRACE(jh, "give to committing trans");
 				ret = __dispose_buffer(jh,
 					journal->j_committing_transaction);
 				jbd2_journal_put_journal_head(jh);
 				spin_unlock(&journal->j_list_lock);
 				jbd_unlock_bh_state(bh);
 				write_unlock(&journal->j_state_lock);
 				return ret;
 			} else {
 				 
 				clear_buffer_jbddirty(bh);
 				goto zap_buffer;
 			}
 		}
 	} else if (transaction == journal->j_committing_transaction) {
 		JBUFFER_TRACE(jh, "on committing transaction");
 		 
 		set_buffer_freed(bh);
 		if (journal->j_running_transaction && buffer_jbddirty(bh))
 			jh->b_next_transaction = journal->j_running_transaction;
 		jbd2_journal_put_journal_head(jh);
 		spin_unlock(&journal->j_list_lock);
 		jbd_unlock_bh_state(bh);
 		write_unlock(&journal->j_state_lock);
 		return 0;
 	} else {
 		 
 		J_ASSERT_JH(jh, transaction == journal->j_running_transaction);
 		JBUFFER_TRACE(jh, "on running transaction");
 		may_free = __dispose_buffer(jh, transaction);
 	}
 
 zap_buffer:
 	jbd2_journal_put_journal_head(jh);
 zap_buffer_no_jh:
 	spin_unlock(&journal->j_list_lock);
 	jbd_unlock_bh_state(bh);
 	write_unlock(&journal->j_state_lock);
 zap_buffer_unlocked:
 	clear_buffer_dirty(bh);
 	J_ASSERT_BH(bh, !buffer_jbddirty(bh));
  	clear_buffer_mapped(bh);
  	clear_buffer_req(bh);
  	clear_buffer_new(bh);
// 	clear_buffer_delay(bh);
// 	clear_buffer_unwritten(bh);
  	bh->b_bdev = NULL;
  	return may_free;
  }