static int fuse_direct_mmap(struct file *file, struct vm_area_struct *vma)
{
	 
	if (vma->vm_flags & VM_MAYSHARE)
		return -ENODEV;

	invalidate_inode_pages2(file->f_mapping);

	return generic_file_mmap(file, vma);
}