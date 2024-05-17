static ssize_t generic_perform_write_2copy(struct file *file,
				struct iov_iter *i, loff_t pos)
{
	struct address_space *mapping = file->f_mapping;
	const struct address_space_operations *a_ops = mapping->a_ops;
	struct inode *inode = mapping->host;
	long status = 0;
	ssize_t written = 0;

	do {
		struct page *src_page;
		struct page *page;
		pgoff_t index;		 
		unsigned long offset;	 
		unsigned long bytes;	 
		size_t copied;		 

		offset = (pos & (PAGE_CACHE_SIZE - 1));
		index = pos >> PAGE_CACHE_SHIFT;
		bytes = min_t(unsigned long, PAGE_CACHE_SIZE - offset,
						iov_iter_count(i));

		 
		src_page = NULL;

		 
		if (unlikely(iov_iter_fault_in_readable(i, bytes))) {
			status = -EFAULT;
			break;
		}

		page = __grab_cache_page(mapping, index);
		if (!page) {
			status = -ENOMEM;
			break;
		}

		 
		if (!PageUptodate(page) && !segment_eq(get_fs(), KERNEL_DS)) {
			unlock_page(page);

			src_page = alloc_page(GFP_KERNEL);
			if (!src_page) {
				page_cache_release(page);
				status = -ENOMEM;
				break;
			}

			 
			copied = iov_iter_copy_from_user(src_page, i,
								offset, bytes);
			if (unlikely(copied == 0)) {
				status = -EFAULT;
				page_cache_release(page);
				page_cache_release(src_page);
				break;
			}
			bytes = copied;

			lock_page(page);
			 
			if (unlikely(!page->mapping || PageUptodate(page))) {
				unlock_page(page);
				page_cache_release(page);
				page_cache_release(src_page);
				continue;
			}
		}

		status = a_ops->prepare_write(file, page, offset, offset+bytes);
		if (unlikely(status))
			goto fs_write_aop_error;

		if (!src_page) {
			 
			pagefault_disable();
			copied = iov_iter_copy_from_user_atomic(page, i,
								offset, bytes);
			pagefault_enable();
		} else {
			void *src, *dst;
			src = kmap_atomic(src_page, KM_USER0);
			dst = kmap_atomic(page, KM_USER1);
			memcpy(dst + offset, src + offset, bytes);
			kunmap_atomic(dst, KM_USER1);
			kunmap_atomic(src, KM_USER0);
			copied = bytes;
		}
		flush_dcache_page(page);

		status = a_ops->commit_write(file, page, offset, offset+bytes);
		if (unlikely(status < 0))
			goto fs_write_aop_error;
		if (unlikely(status > 0))  
			copied = min_t(size_t, copied, status);

		unlock_page(page);
		mark_page_accessed(page);
		page_cache_release(page);
		if (src_page)
			page_cache_release(src_page);

		iov_iter_advance(i, copied);
		pos += copied;
		written += copied;

		balance_dirty_pages_ratelimited(mapping);
		cond_resched();
		continue;

fs_write_aop_error:
		unlock_page(page);
		page_cache_release(page);
		if (src_page)
			page_cache_release(src_page);

		 
		if (pos + bytes > inode->i_size)
			vmtruncate(inode, inode->i_size);
		break;
	} while (iov_iter_count(i));

	return written ? written : status;
}
