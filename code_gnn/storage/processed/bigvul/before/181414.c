 static ssize_t read_mem(struct file *file, char __user *buf,
 			size_t count, loff_t *ppos)
 {
 	phys_addr_t p = *ppos;
 	ssize_t read, sz;
 	void *ptr;
 
 	if (p != *ppos)
 		return 0;
 
 	if (!valid_phys_addr_range(p, count))
 		return -EFAULT;
 	read = 0;
 #ifdef __ARCH_HAS_NO_PAGE_ZERO_MAPPED
 	 
 	if (p < PAGE_SIZE) {
 		sz = size_inside_page(p, count);
 		if (sz > 0) {
 			if (clear_user(buf, sz))
 				return -EFAULT;
 			buf += sz;
 			p += sz;
 			count -= sz;
 			read += sz;
 		}
 	}
 #endif
  
  	while (count > 0) {
  		unsigned long remaining;
// 		int allowed;
  
  		sz = size_inside_page(p, count);
  
		if (!range_is_allowed(p >> PAGE_SHIFT, count))
// 		allowed = page_is_allowed(p >> PAGE_SHIFT);
// 		if (!allowed)
  			return -EPERM;
// 		if (allowed == 2) {
// 			 
// 			remaining = clear_user(buf, sz);
// 		} else {
// 			 
// 			ptr = xlate_dev_mem_ptr(p);
// 			if (!ptr)
// 				return -EFAULT;
  
		 
		ptr = xlate_dev_mem_ptr(p);
		if (!ptr)
			return -EFAULT;
// 			remaining = copy_to_user(buf, ptr, sz);
// 
// 			unxlate_dev_mem_ptr(p, ptr);
// 		}
  
		remaining = copy_to_user(buf, ptr, sz);
		unxlate_dev_mem_ptr(p, ptr);
  		if (remaining)
  			return -EFAULT;
  
 		buf += sz;
 		p += sz;
 		count -= sz;
 		read += sz;
 	}
 
 	*ppos += read;
 	return read;
 }