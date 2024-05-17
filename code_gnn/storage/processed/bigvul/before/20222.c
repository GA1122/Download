static int hugetlbfs_file_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct inode *inode = file->f_path.dentry->d_inode;
	loff_t len, vma_len;
	int ret;
	struct hstate *h = hstate_file(file);

	 
	vma->vm_flags |= VM_HUGETLB | VM_RESERVED;
	vma->vm_ops = &hugetlb_vm_ops;

	if (vma->vm_pgoff & (~huge_page_mask(h) >> PAGE_SHIFT))
		return -EINVAL;

	vma_len = (loff_t)(vma->vm_end - vma->vm_start);

	mutex_lock(&inode->i_mutex);
	file_accessed(file);

	ret = -ENOMEM;
	len = vma_len + ((loff_t)vma->vm_pgoff << PAGE_SHIFT);

	if (hugetlb_reserve_pages(inode,
				vma->vm_pgoff >> huge_page_order(h),
				len >> huge_page_shift(h), vma,
				vma->vm_flags))
		goto out;

	ret = 0;
	hugetlb_prefault_arch_hook(vma->vm_mm);
	if (vma->vm_flags & VM_WRITE && inode->i_size < len)
		inode->i_size = len;
out:
	mutex_unlock(&inode->i_mutex);

	return ret;
}