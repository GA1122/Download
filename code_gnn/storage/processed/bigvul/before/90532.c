static int __vm_munmap(unsigned long start, size_t len, bool downgrade)
{
	int ret;
	struct mm_struct *mm = current->mm;
	LIST_HEAD(uf);

	if (down_write_killable(&mm->mmap_sem))
		return -EINTR;

	ret = __do_munmap(mm, start, len, &uf, downgrade);
	 
	if (ret == 1) {
		up_read(&mm->mmap_sem);
		ret = 0;
	} else
		up_write(&mm->mmap_sem);

	userfaultfd_unmap_complete(mm, &uf);
	return ret;
}
