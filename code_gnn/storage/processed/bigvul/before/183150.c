 ext2_xattr_cache_insert(struct buffer_head *bh)
// ext2_xattr_cache_insert(struct mb2_cache *cache, struct buffer_head *bh)
  {
  	__u32 hash = le32_to_cpu(HDR(bh)->h_hash);
	struct mb_cache_entry *ce;
  	int error;
  
	ce = mb_cache_entry_alloc(ext2_xattr_cache, GFP_NOFS);
	if (!ce)
		return -ENOMEM;
	error = mb_cache_entry_insert(ce, bh->b_bdev, bh->b_blocknr, hash);
// 	error = mb2_cache_entry_create(cache, GFP_NOFS, hash, bh->b_blocknr);
  	if (error) {
		mb_cache_entry_free(ce);
  		if (error == -EBUSY) {
  			ea_bdebug(bh, "already in cache (%d cache entries)",
  				atomic_read(&ext2_xattr_cache->c_entry_count));
  			error = 0;
  		}
	} else {
		ea_bdebug(bh, "inserting [%x] (%d cache entries)", (int)hash,
			  atomic_read(&ext2_xattr_cache->c_entry_count));
		mb_cache_entry_release(ce);
	}
// 	} else
// 		ea_bdebug(bh, "inserting [%x]", (int)hash);
  	return error;
  }