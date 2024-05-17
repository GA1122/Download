void exit_robust_list(struct task_struct *curr)
{
	struct robust_list_head __user *head = curr->robust_list;
	struct robust_list __user *entry, *next_entry, *pending;
	unsigned int limit = ROBUST_LIST_LIMIT, pi, pip;
	unsigned int uninitialized_var(next_pi);
	unsigned long futex_offset;
	int rc;

	if (!futex_cmpxchg_enabled)
		return;

	 
	if (fetch_robust_entry(&entry, &head->list.next, &pi))
		return;
	 
	if (get_user(futex_offset, &head->futex_offset))
		return;
	 
	if (fetch_robust_entry(&pending, &head->list_op_pending, &pip))
		return;

	next_entry = NULL;	 
	while (entry != &head->list) {
		 
		rc = fetch_robust_entry(&next_entry, &entry->next, &next_pi);
		 
		if (entry != pending)
			if (handle_futex_death((void __user *)entry + futex_offset,
						curr, pi))
				return;
		if (rc)
			return;
		entry = next_entry;
		pi = next_pi;
		 
		if (!--limit)
			break;

		cond_resched();
	}

	if (pending)
		handle_futex_death((void __user *)pending + futex_offset,
				   curr, pip);
}
