static int mmap_kmem(struct file *file, struct vm_area_struct *vma)
{
	unsigned long pfn;

	 
	pfn = __pa((u64)vma->vm_pgoff << PAGE_SHIFT) >> PAGE_SHIFT;

	 
	if (!pfn_valid(pfn))
		return -EIO;

	vma->vm_pgoff = pfn;
	return mmap_mem(file, vma);
}
