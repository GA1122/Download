static void key_garbage_collector(struct work_struct *work)
{
	static LIST_HEAD(graveyard);
	static u8 gc_state;		 
#define KEY_GC_REAP_AGAIN	0x01	 
#define KEY_GC_REAPING_LINKS	0x02	 
#define KEY_GC_SET_TIMER	0x04	 
#define KEY_GC_REAPING_DEAD_1	0x10	 
#define KEY_GC_REAPING_DEAD_2	0x20	 
#define KEY_GC_REAPING_DEAD_3	0x40	 
#define KEY_GC_FOUND_DEAD_KEY	0x80	 

	struct rb_node *cursor;
	struct key *key;
	time_t new_timer, limit;

	kenter("[%lx,%x]", key_gc_flags, gc_state);

	limit = current_kernel_time().tv_sec;
	if (limit > key_gc_delay)
		limit -= key_gc_delay;
	else
		limit = key_gc_delay;

	 
	gc_state &= KEY_GC_REAPING_DEAD_1 | KEY_GC_REAPING_DEAD_2;
	gc_state <<= 1;
	if (test_and_clear_bit(KEY_GC_KEY_EXPIRED, &key_gc_flags))
		gc_state |= KEY_GC_REAPING_LINKS | KEY_GC_SET_TIMER;

	if (test_and_clear_bit(KEY_GC_REAP_KEYTYPE, &key_gc_flags))
		gc_state |= KEY_GC_REAPING_DEAD_1;
	kdebug("new pass %x", gc_state);

	new_timer = LONG_MAX;

	 
	spin_lock(&key_serial_lock);
	cursor = rb_first(&key_serial_tree);

continue_scanning:
	while (cursor) {
		key = rb_entry(cursor, struct key, serial_node);
		cursor = rb_next(cursor);

		if (refcount_read(&key->usage) == 0)
			goto found_unreferenced_key;

		if (unlikely(gc_state & KEY_GC_REAPING_DEAD_1)) {
			if (key->type == key_gc_dead_keytype) {
				gc_state |= KEY_GC_FOUND_DEAD_KEY;
				set_bit(KEY_FLAG_DEAD, &key->flags);
				key->perm = 0;
				goto skip_dead_key;
			} else if (key->type == &key_type_keyring &&
				   key->restrict_link) {
				goto found_restricted_keyring;
			}
		}

		if (gc_state & KEY_GC_SET_TIMER) {
			if (key->expiry > limit && key->expiry < new_timer) {
				kdebug("will expire %x in %ld",
				       key_serial(key), key->expiry - limit);
				new_timer = key->expiry;
			}
		}

		if (unlikely(gc_state & KEY_GC_REAPING_DEAD_2))
			if (key->type == key_gc_dead_keytype)
				gc_state |= KEY_GC_FOUND_DEAD_KEY;

		if ((gc_state & KEY_GC_REAPING_LINKS) ||
		    unlikely(gc_state & KEY_GC_REAPING_DEAD_2)) {
			if (key->type == &key_type_keyring)
				goto found_keyring;
		}

		if (unlikely(gc_state & KEY_GC_REAPING_DEAD_3))
			if (key->type == key_gc_dead_keytype)
				goto destroy_dead_key;

	skip_dead_key:
		if (spin_is_contended(&key_serial_lock) || need_resched())
			goto contended;
	}

contended:
	spin_unlock(&key_serial_lock);

maybe_resched:
	if (cursor) {
		cond_resched();
		spin_lock(&key_serial_lock);
		goto continue_scanning;
	}

	 
	kdebug("pass complete");

	if (gc_state & KEY_GC_SET_TIMER && new_timer != (time_t)LONG_MAX) {
		new_timer += key_gc_delay;
		key_schedule_gc(new_timer);
	}

	if (unlikely(gc_state & KEY_GC_REAPING_DEAD_2) ||
	    !list_empty(&graveyard)) {
		 
		kdebug("gc sync");
		synchronize_rcu();
	}

	if (!list_empty(&graveyard)) {
		kdebug("gc keys");
		key_gc_unused_keys(&graveyard);
	}

	if (unlikely(gc_state & (KEY_GC_REAPING_DEAD_1 |
				 KEY_GC_REAPING_DEAD_2))) {
		if (!(gc_state & KEY_GC_FOUND_DEAD_KEY)) {
			 
			kdebug("dead short");
			gc_state &= ~(KEY_GC_REAPING_DEAD_1 | KEY_GC_REAPING_DEAD_2);
			gc_state |= KEY_GC_REAPING_DEAD_3;
		} else {
			gc_state |= KEY_GC_REAP_AGAIN;
		}
	}

	if (unlikely(gc_state & KEY_GC_REAPING_DEAD_3)) {
		kdebug("dead wake");
		smp_mb();
		clear_bit(KEY_GC_REAPING_KEYTYPE, &key_gc_flags);
		wake_up_bit(&key_gc_flags, KEY_GC_REAPING_KEYTYPE);
	}

	if (gc_state & KEY_GC_REAP_AGAIN)
		schedule_work(&key_gc_work);
	kleave(" [end %x]", gc_state);
	return;

	 
found_unreferenced_key:
	kdebug("unrefd key %d", key->serial);
	rb_erase(&key->serial_node, &key_serial_tree);
	spin_unlock(&key_serial_lock);

	list_add_tail(&key->graveyard_link, &graveyard);
	gc_state |= KEY_GC_REAP_AGAIN;
	goto maybe_resched;

	 
found_restricted_keyring:
	spin_unlock(&key_serial_lock);
	keyring_restriction_gc(key, key_gc_dead_keytype);
	goto maybe_resched;

	 
found_keyring:
	spin_unlock(&key_serial_lock);
	keyring_gc(key, limit);
	goto maybe_resched;

	 
destroy_dead_key:
	spin_unlock(&key_serial_lock);
	kdebug("destroy key %d", key->serial);
	down_write(&key->sem);
	key->type = &key_type_dead;
	if (key_gc_dead_keytype->destroy)
		key_gc_dead_keytype->destroy(key);
	memset(&key->payload, KEY_DESTROY, sizeof(key->payload));
	up_write(&key->sem);
	goto maybe_resched;
}
