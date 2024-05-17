  void user_describe(const struct key *key, struct seq_file *m)
  {
  	seq_puts(m, key->description);
	if (key_is_instantiated(key))
// 	if (key_is_positive(key))
  		seq_printf(m, ": %u", key->datalen);
  }