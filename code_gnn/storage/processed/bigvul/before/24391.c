int jbd2_journal_stop(handle_t *handle)
{
	transaction_t *transaction = handle->h_transaction;
	journal_t *journal = transaction->t_journal;
	int err, wait_for_commit = 0;
	tid_t tid;
	pid_t pid;

	J_ASSERT(journal_current_handle() == handle);

	if (is_handle_aborted(handle))
		err = -EIO;
	else {
		J_ASSERT(atomic_read(&transaction->t_updates) > 0);
		err = 0;
	}

	if (--handle->h_ref > 0) {
		jbd_debug(4, "h_ref %d -> %d\n", handle->h_ref + 1,
			  handle->h_ref);
		return err;
	}

	jbd_debug(4, "Handle %p going down\n", handle);

	 
	pid = current->pid;
	if (handle->h_sync && journal->j_last_sync_writer != pid) {
		u64 commit_time, trans_time;

		journal->j_last_sync_writer = pid;

		read_lock(&journal->j_state_lock);
		commit_time = journal->j_average_commit_time;
		read_unlock(&journal->j_state_lock);

		trans_time = ktime_to_ns(ktime_sub(ktime_get(),
						   transaction->t_start_time));

		commit_time = max_t(u64, commit_time,
				    1000*journal->j_min_batch_time);
		commit_time = min_t(u64, commit_time,
				    1000*journal->j_max_batch_time);

		if (trans_time < commit_time) {
			ktime_t expires = ktime_add_ns(ktime_get(),
						       commit_time);
			set_current_state(TASK_UNINTERRUPTIBLE);
			schedule_hrtimeout(&expires, HRTIMER_MODE_ABS);
		}
	}

	if (handle->h_sync)
		transaction->t_synchronous_commit = 1;
	current->journal_info = NULL;
	atomic_sub(handle->h_buffer_credits,
		   &transaction->t_outstanding_credits);

	 
	if (handle->h_sync ||
	    (atomic_read(&transaction->t_outstanding_credits) >
	     journal->j_max_transaction_buffers) ||
	    time_after_eq(jiffies, transaction->t_expires)) {
		 

		jbd_debug(2, "transaction too old, requesting commit for "
					"handle %p\n", handle);
		 
		jbd2_log_start_commit(journal, transaction->t_tid);

		 
		if (handle->h_sync && !(current->flags & PF_MEMALLOC))
			wait_for_commit = 1;
	}

	 
	tid = transaction->t_tid;
	if (atomic_dec_and_test(&transaction->t_updates)) {
		wake_up(&journal->j_wait_updates);
		if (journal->j_barrier_count)
			wake_up(&journal->j_wait_transaction_locked);
	}

	if (wait_for_commit)
		err = jbd2_log_wait_commit(journal, tid);

	lock_map_release(&handle->h_lockdep_map);

	jbd2_free_handle(handle);
	return err;
}
