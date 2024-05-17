int bio_iov_iter_get_pages(struct bio *bio, struct iov_iter *iter)
{
	unsigned short nr_pages = bio->bi_max_vecs - bio->bi_vcnt;
	struct bio_vec *bv = bio->bi_io_vec + bio->bi_vcnt;
	struct page **pages = (struct page **)bv;
	size_t offset, diff;
	ssize_t size;

	size = iov_iter_get_pages(iter, pages, LONG_MAX, nr_pages, &offset);
	if (unlikely(size <= 0))
		return size ? size : -EFAULT;
	nr_pages = (size + offset + PAGE_SIZE - 1) / PAGE_SIZE;

	 
	bio->bi_iter.bi_size += size;
	bio->bi_vcnt += nr_pages;

	diff = (nr_pages * PAGE_SIZE - offset) - size;
	while (nr_pages--) {
		bv[nr_pages].bv_page = pages[nr_pages];
		bv[nr_pages].bv_len = PAGE_SIZE;
		bv[nr_pages].bv_offset = 0;
	}

	bv[0].bv_offset += offset;
	bv[0].bv_len -= offset;
	if (diff)
		bv[bio->bi_vcnt - 1].bv_len -= diff;

	iov_iter_advance(iter, size);
	return 0;
}
