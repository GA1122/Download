cifs_readdata_to_iov(struct cifs_readdata *rdata, const struct iovec *iov,
			unsigned long nr_segs, loff_t offset, ssize_t *copied)
{
	int rc = 0;
	struct iov_iter ii;
	size_t pos = rdata->offset - offset;
	ssize_t remaining = rdata->bytes;
	unsigned char *pdata;
	unsigned int i;

	 
	iov_iter_init(&ii, iov, nr_segs, iov_length(iov, nr_segs), 0);
	iov_iter_advance(&ii, pos);

	*copied = 0;
	for (i = 0; i < rdata->nr_pages; i++) {
		ssize_t copy;
		struct page *page = rdata->pages[i];

		 
		copy = min_t(ssize_t, remaining, PAGE_SIZE);

		 
		copy = min_t(ssize_t, copy, iov_iter_count(&ii));

		 
		if (copy && !rc) {
			pdata = kmap(page);
			rc = memcpy_toiovecend(ii.iov, pdata, ii.iov_offset,
						(int)copy);
			kunmap(page);
			if (!rc) {
				*copied += copy;
				remaining -= copy;
				iov_iter_advance(&ii, copy);
			}
		}
	}

	return rc;
}
