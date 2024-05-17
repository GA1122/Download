static int lookup_pi_state(u32 __user *uaddr, u32 uval,
			   struct futex_hash_bucket *hb,
			   union futex_key *key, struct futex_pi_state **ps)
{
	struct futex_q *top_waiter = futex_top_waiter(hb, key);

	 
	if (top_waiter)
		return attach_to_pi_state(uaddr, uval, top_waiter->pi_state, ps);

	 
	return attach_to_pi_owner(uval, key, ps);
}
