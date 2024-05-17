futex_wake(u32 __user *uaddr, unsigned int flags, int nr_wake, u32 bitset)
{
	struct futex_hash_bucket *hb;
	struct futex_q *this, *next;
	struct plist_head *head;
	union futex_key key = FUTEX_KEY_INIT;
	int ret;

	if (!bitset)
		return -EINVAL;

	ret = get_futex_key(uaddr, flags & FLAGS_SHARED, &key, VERIFY_READ);
	if (unlikely(ret != 0))
		goto out;

	hb = hash_futex(&key);
	spin_lock(&hb->lock);
	head = &hb->chain;

	plist_for_each_entry_safe(this, next, head, list) {
		if (match_futex (&this->key, &key)) {
			if (this->pi_state || this->rt_waiter) {
				ret = -EINVAL;
				break;
			}

			 
			if (!(this->bitset & bitset))
				continue;

			wake_futex(this);
			if (++ret >= nr_wake)
				break;
		}
	}

	spin_unlock(&hb->lock);
	put_futex_key(&key);
out:
	return ret;
}