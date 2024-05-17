  ext2_xattr_cache_find(struct inode *inode, struct ext2_xattr_header *header)
  {
  	__u32 hash = le32_to_cpu(header->h_hash);
	struct mb_cache_entry *ce;
// 	struct mb2_cache_entry *ce;
// 	struct mb2_cache *ext2_mb_cache = EXT2_SB(inode->i_sb)->s_mb_cache;
  
  	if (!header->h_hash)
  		return NULL;   
  	ea_idebug(inode, "looking for cached blocks [%x]", (int)hash);
  again:
	ce = mb_cache_entry_find_first(ext2_xattr_cache, inode->i_sb->s_bdev,
				       hash);
// 	ce = mb2_cache_entry_find_first(ext2_mb_cache, hash);
  	while (ce) {
  		struct buffer_head *bh;
  
		if (IS_ERR(ce)) {
			if (PTR_ERR(ce) == -EAGAIN)
				goto again;
			break;
		}
  		bh = sb_bread(inode->i_sb, ce->e_block);
  		if (!bh) {
  			ext2_error(inode->i_sb, "ext2_xattr_cache_find",
  				"inode %ld: block %ld read error",
  				inode->i_ino, (unsigned long) ce->e_block);
  		} else {
  			lock_buffer(bh);
			if (le32_to_cpu(HDR(bh)->h_refcount) >
// 			 
// 			if (hlist_bl_unhashed(&ce->e_hash_list)) {
// 				mb2_cache_entry_put(ext2_mb_cache, ce);
// 				unlock_buffer(bh);
// 				brelse(bh);
// 				goto again;
// 			} else if (le32_to_cpu(HDR(bh)->h_refcount) >
  				   EXT2_XATTR_REFCOUNT_MAX) {
  				ea_idebug(inode, "block %ld refcount %d>%d",
  					  (unsigned long) ce->e_block,
 					  le32_to_cpu(HDR(bh)->h_refcount),
 					  EXT2_XATTR_REFCOUNT_MAX);
  			} else if (!ext2_xattr_cmp(header, HDR(bh))) {
  				ea_bdebug(bh, "b_count=%d",
  					  atomic_read(&(bh->b_count)));
				mb_cache_entry_release(ce);
// 				mb2_cache_entry_touch(ext2_mb_cache, ce);
// 				mb2_cache_entry_put(ext2_mb_cache, ce);
  				return bh;
  			}
  			unlock_buffer(bh);
  			brelse(bh);
  		}
		ce = mb_cache_entry_find_next(ce, inode->i_sb->s_bdev, hash);
// 		ce = mb2_cache_entry_find_next(ext2_mb_cache, ce);
  	}
  	return NULL;
  }