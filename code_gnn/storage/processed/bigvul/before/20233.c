hugetlbfs_read_actor(struct page *page, unsigned long offset,
			char __user *buf, unsigned long count,
			unsigned long size)
{
	char *kaddr;
	unsigned long left, copied = 0;
	int i, chunksize;

	if (size > count)
		size = count;

	 
	i = offset >> PAGE_CACHE_SHIFT;
	offset = offset & ~PAGE_CACHE_MASK;

	while (size) {
		chunksize = PAGE_CACHE_SIZE;
		if (offset)
			chunksize -= offset;
		if (chunksize > size)
			chunksize = size;
		kaddr = kmap(&page[i]);
		left = __copy_to_user(buf, kaddr + offset, chunksize);
		kunmap(&page[i]);
		if (left) {
			copied += (chunksize - left);
			break;
		}
		offset = 0;
		size -= chunksize;
		buf += chunksize;
		copied += chunksize;
		i++;
	}
	return copied ? copied : -EFAULT;
}