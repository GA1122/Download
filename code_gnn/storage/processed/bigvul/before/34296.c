static void btrfs_endio_direct_read(struct bio *bio, int err)
{
	struct btrfs_dio_private *dip = bio->bi_private;
	struct bio_vec *bvec_end = bio->bi_io_vec + bio->bi_vcnt - 1;
	struct bio_vec *bvec = bio->bi_io_vec;
	struct inode *inode = dip->inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	u64 start;

	start = dip->logical_offset;
	do {
		if (!(BTRFS_I(inode)->flags & BTRFS_INODE_NODATASUM)) {
			struct page *page = bvec->bv_page;
			char *kaddr;
			u32 csum = ~(u32)0;
			u64 private = ~(u32)0;
			unsigned long flags;

			if (get_state_private(&BTRFS_I(inode)->io_tree,
					      start, &private))
				goto failed;
			local_irq_save(flags);
			kaddr = kmap_atomic(page);
			csum = btrfs_csum_data(root, kaddr + bvec->bv_offset,
					       csum, bvec->bv_len);
			btrfs_csum_final(csum, (char *)&csum);
			kunmap_atomic(kaddr);
			local_irq_restore(flags);

			flush_dcache_page(bvec->bv_page);
			if (csum != private) {
failed:
				printk(KERN_ERR "btrfs csum failed ino %llu off"
				      " %llu csum %u private %u\n",
				      (unsigned long long)btrfs_ino(inode),
				      (unsigned long long)start,
				      csum, (unsigned)private);
				err = -EIO;
			}
		}

		start += bvec->bv_len;
		bvec++;
	} while (bvec <= bvec_end);

	unlock_extent(&BTRFS_I(inode)->io_tree, dip->logical_offset,
		      dip->logical_offset + dip->bytes - 1);
	bio->bi_private = dip->private;

	kfree(dip);

	 
	if (err)
		clear_bit(BIO_UPTODATE, &bio->bi_flags);
	dio_end_io(bio, err);
}