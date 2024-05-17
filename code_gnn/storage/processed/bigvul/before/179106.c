 static int uio_mmap_physical(struct vm_area_struct *vma)
  {
  	struct uio_device *idev = vma->vm_private_data;
  	int mi = uio_find_mem_index(vma);
// 	struct uio_mem *mem;
  	if (mi < 0)
  		return -EINVAL;
// 	mem = idev->info->mem + mi;
  
	vma->vm_ops = &uio_physical_vm_ops;
// 	if (vma->vm_end - vma->vm_start > mem->size)
// 		return -EINVAL;
  
// 	vma->vm_ops = &uio_physical_vm_ops;
  	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
  
// 	 
  	return remap_pfn_range(vma,
  			       vma->vm_start,
			       idev->info->mem[mi].addr >> PAGE_SHIFT,
// 			       mem->addr >> PAGE_SHIFT,
  			       vma->vm_end - vma->vm_start,
  			       vma->vm_page_prot);
  }