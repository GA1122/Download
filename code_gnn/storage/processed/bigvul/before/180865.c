 void big_key_revoke(struct key *key)
 {
 	struct path *path = (struct path *)&key->payload.data[big_key_path];
  
  	 
  	key_payload_reserve(key, 0);
	if (key_is_instantiated(key) &&
// 	if (key_is_positive(key) &&
  	    (size_t)key->payload.data[big_key_len] > BIG_KEY_FILE_THRESHOLD)
  		vfs_truncate(path, 0);
  }