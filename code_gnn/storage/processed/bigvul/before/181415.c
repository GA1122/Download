 static ssize_t write_mem(struct file *file, const char __user *buf,
 			 size_t count, loff_t *ppos)
 {
 	phys_addr_t p = *ppos;
 	ssize_t written, sz;
 	unsigned long copied;
 	void *ptr;
 
 	if (p != *ppos)
 		return -EFBIG;
 
 	if (!valid_phys_addr_range(p, count))
 		return -EFAULT;
 
 	written = 0;
 
 #ifdef __ARCH_HAS_NO_PAGE_ZERO_MAPPED
 	 
 	if (p < PAGE_SIZE) {
 		sz = size_inside_page(p, count);
 		 
 		buf += sz;
 		p += sz;
 		count -= sz;
 		written += sz;
 	}
  #endif
  
  	while (count > 0) {
// 		int allowed;
// 
  		sz = size_inside_page(p, count);
  
		if (!range_is_allowed(p >> PAGE_SHIFT, sz))
// 		allowed = page_is_allowed(p >> PAGE_SHIFT);
// 		if (!allowed)
  			return -EPERM;
  
 		 
		ptr = xlate_dev_mem_ptr(p);
		if (!ptr) {
			if (written)
				break;
			return -EFAULT;
		}
// 		if (allowed == 1) {
// 			 
// 			ptr = xlate_dev_mem_ptr(p);
// 			if (!ptr) {
// 				if (written)
// 					break;
// 				return -EFAULT;
// 			}
  
		copied = copy_from_user(ptr, buf, sz);
		unxlate_dev_mem_ptr(p, ptr);
		if (copied) {
			written += sz - copied;
			if (written)
				break;
			return -EFAULT;
// 			copied = copy_from_user(ptr, buf, sz);
// 			unxlate_dev_mem_ptr(p, ptr);
// 			if (copied) {
// 				written += sz - copied;
// 				if (written)
// 					break;
// 				return -EFAULT;
// 			}
  		}
  
  		buf += sz;
 		p += sz;
 		count -= sz;
 		written += sz;
 	}
 
 	*ppos += written;
 	return written;
 }