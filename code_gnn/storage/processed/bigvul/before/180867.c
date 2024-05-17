 static noinline void key_gc_unused_keys(struct list_head *keys)
 {
  	while (!list_empty(keys)) {
  		struct key *key =
  			list_entry(keys->next, struct key, graveyard_link);
// 		short state = key->state;
// 
  		list_del(&key->graveyard_link);
  
  		kdebug("- %u", key->serial);
  		key_check(key);
  
  		 
		if (test_bit(KEY_FLAG_INSTANTIATED, &key->flags) &&
		    !test_bit(KEY_FLAG_NEGATIVE, &key->flags) &&
		    key->type->destroy)
// 		if (state == KEY_IS_POSITIVE && key->type->destroy)
  			key->type->destroy(key);
  
  		security_key_free(key);
 
 		 
 		if (test_bit(KEY_FLAG_IN_QUOTA, &key->flags)) {
 			spin_lock(&key->user->lock);
 			key->user->qnkeys--;
 			key->user->qnbytes -= key->quotalen;
 			spin_unlock(&key->user->lock);
  		}
  
  		atomic_dec(&key->user->nkeys);
		if (test_bit(KEY_FLAG_INSTANTIATED, &key->flags))
// 		if (state != KEY_IS_UNINSTANTIATED)
  			atomic_dec(&key->user->nikeys);
  
  		key_user_put(key->user);
 
 		kfree(key->description);
 
 		memzero_explicit(key, sizeof(*key));
 		kmem_cache_free(key_jar, key);
 	}
 }