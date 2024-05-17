static void ext4_update_bh_state(struct buffer_head *bh, unsigned long flags)
{
	unsigned long old_state;
	unsigned long new_state;

	flags &= EXT4_MAP_FLAGS;

	 
	if (!bh->b_page) {
		bh->b_state = (bh->b_state & ~EXT4_MAP_FLAGS) | flags;
		return;
	}
	 
	do {
		old_state = READ_ONCE(bh->b_state);
		new_state = (old_state & ~EXT4_MAP_FLAGS) | flags;
	} while (unlikely(
		 cmpxchg(&bh->b_state, old_state, new_state) != old_state));
}
