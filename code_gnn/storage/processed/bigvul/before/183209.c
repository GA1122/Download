 struct bio *bio_map_user_iov(struct request_queue *q,
 			     const struct iov_iter *iter,
 			     gfp_t gfp_mask)
 {
 	int j;
 	int nr_pages = 0;
 	struct page **pages;
 	struct bio *bio;
 	int cur_page = 0;
  	int ret, offset;
  	struct iov_iter i;
  	struct iovec iov;
// 	struct bio_vec *bvec;
  
  	iov_for_each(iov, i, *iter) {
  		unsigned long uaddr = (unsigned long) iov.iov_base;
 		unsigned long len = iov.iov_len;
 		unsigned long end = (uaddr + len + PAGE_SIZE - 1) >> PAGE_SHIFT;
 		unsigned long start = uaddr >> PAGE_SHIFT;
 
 		 
 		if (end < start)
 			return ERR_PTR(-EINVAL);
 
 		nr_pages += end - start;
 		 
 		if (uaddr & queue_dma_alignment(q))
 			return ERR_PTR(-EINVAL);
 	}
 
 	if (!nr_pages)
 		return ERR_PTR(-EINVAL);
 
 	bio = bio_kmalloc(gfp_mask, nr_pages);
 	if (!bio)
 		return ERR_PTR(-ENOMEM);
 
 	ret = -ENOMEM;
 	pages = kcalloc(nr_pages, sizeof(struct page *), gfp_mask);
 	if (!pages)
 		goto out;
 
 	iov_for_each(iov, i, *iter) {
 		unsigned long uaddr = (unsigned long) iov.iov_base;
 		unsigned long len = iov.iov_len;
 		unsigned long end = (uaddr + len + PAGE_SIZE - 1) >> PAGE_SHIFT;
 		unsigned long start = uaddr >> PAGE_SHIFT;
 		const int local_nr_pages = end - start;
 		const int page_limit = cur_page + local_nr_pages;
 
  		ret = get_user_pages_fast(uaddr, local_nr_pages,
  				(iter->type & WRITE) != WRITE,
  				&pages[cur_page]);
		if (ret < local_nr_pages) {
// 		if (unlikely(ret < local_nr_pages)) {
// 			for (j = cur_page; j < page_limit; j++) {
// 				if (!pages[j])
// 					break;
// 				put_page(pages[j]);
// 			}
  			ret = -EFAULT;
  			goto out_unmap;
  		}
 
 		offset = offset_in_page(uaddr);
 		for (j = cur_page; j < page_limit; j++) {
 			unsigned int bytes = PAGE_SIZE - offset;
 			unsigned short prev_bi_vcnt = bio->bi_vcnt;
 
 			if (len <= 0)
 				break;
 			
 			if (bytes > len)
 				bytes = len;
 
 			 
 			if (bio_add_pc_page(q, bio, pages[j], bytes, offset) <
 					    bytes)
 				break;
 
 			 
 			if (bio->bi_vcnt == prev_bi_vcnt)
 				put_page(pages[j]);
 
 			len -= bytes;
 			offset = 0;
 		}
 
 		cur_page = j;
 		 
 		while (j < page_limit)
 			put_page(pages[j++]);
 	}
 
 	kfree(pages);
 
 	bio_set_flag(bio, BIO_USER_MAPPED);
 
 	 
 	bio_get(bio);
  	return bio;
  
   out_unmap:
	for (j = 0; j < nr_pages; j++) {
		if (!pages[j])
			break;
		put_page(pages[j]);
// 	bio_for_each_segment_all(bvec, bio, j) {
// 		put_page(bvec->bv_page);
  	}
   out:
  	kfree(pages);
 	bio_put(bio);
 	return ERR_PTR(ret);
 }