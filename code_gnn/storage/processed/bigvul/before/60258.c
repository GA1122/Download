	__acquires(key_user_lock)
{
	struct rb_node *_p;
	loff_t pos = *_pos;

	spin_lock(&key_user_lock);

	_p = key_user_first(seq_user_ns(p), &key_user_tree);
	while (pos > 0 && _p) {
		pos--;
		_p = key_user_next(seq_user_ns(p), _p);
	}

	return _p;
}