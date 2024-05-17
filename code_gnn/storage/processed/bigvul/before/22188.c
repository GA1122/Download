	__acquires(rose_list_lock)
{
	spin_lock_bh(&rose_list_lock);
	return seq_hlist_start_head(&rose_list, *pos);
}
