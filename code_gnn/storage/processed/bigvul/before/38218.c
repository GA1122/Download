int handle_futex_death(u32 __user *uaddr, struct task_struct *curr, int pi)
{
	u32 uval, uninitialized_var(nval), mval;

retry:
	if (get_user(uval, uaddr))
		return -1;

	if ((uval & FUTEX_TID_MASK) == task_pid_vnr(curr)) {
		 
		mval = (uval & FUTEX_WAITERS) | FUTEX_OWNER_DIED;
		 
		if (cmpxchg_futex_value_locked(&nval, uaddr, uval, mval)) {
			if (fault_in_user_writeable(uaddr))
				return -1;
			goto retry;
		}
		if (nval != uval)
			goto retry;

		 
		if (!pi && (uval & FUTEX_WAITERS))
			futex_wake(uaddr, 1, 1, FUTEX_BITSET_MATCH_ANY);
	}
	return 0;
}
