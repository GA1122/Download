static int nfs4_reclaim_open_state(struct nfs4_state_owner *sp, const struct nfs4_state_recovery_ops *ops)
{
	struct nfs4_state *state;
	struct nfs4_lock_state *lock;
	int status = 0;

	 
restart:
	spin_lock(&sp->so_lock);
	list_for_each_entry(state, &sp->so_states, open_states) {
		if (!test_and_clear_bit(ops->state_flag_bit, &state->flags))
			continue;
		if (state->state == 0)
			continue;
		atomic_inc(&state->count);
		spin_unlock(&sp->so_lock);
		status = ops->recover_open(sp, state);
		if (status >= 0) {
			status = nfs4_reclaim_locks(state, ops);
			if (status >= 0) {
				list_for_each_entry(lock, &state->lock_states, ls_locks) {
					if (!(lock->ls_flags & NFS_LOCK_INITIALIZED))
						printk("%s: Lock reclaim failed!\n",
							__func__);
				}
				nfs4_put_open_state(state);
				goto restart;
			}
		}
		switch (status) {
			default:
				printk(KERN_ERR "%s: unhandled error %d. Zeroing state\n",
						__func__, status);
			case -ENOENT:
			case -ESTALE:
				 
				memset(state->stateid.data, 0,
					sizeof(state->stateid.data));
				 
				state->state = 0;
				break;
			case -NFS4ERR_RECLAIM_BAD:
			case -NFS4ERR_RECLAIM_CONFLICT:
				nfs4_state_mark_reclaim_nograce(sp->so_client, state);
				break;
			case -NFS4ERR_EXPIRED:
			case -NFS4ERR_NO_GRACE:
				nfs4_state_mark_reclaim_nograce(sp->so_client, state);
			case -NFS4ERR_STALE_CLIENTID:
				goto out_err;
		}
		nfs4_put_open_state(state);
		goto restart;
	}
	spin_unlock(&sp->so_lock);
	return 0;
out_err:
	nfs4_put_open_state(state);
	return status;
}
