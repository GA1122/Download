static ssize_t pagemap_read(struct file *file, char __user *buf,
			    size_t count, loff_t *ppos)
{
	struct task_struct *task = get_proc_task(file->f_path.dentry->d_inode);
	struct mm_struct *mm;
	struct pagemapread pm;
	int ret = -ESRCH;
	struct mm_walk pagemap_walk = {};
	unsigned long src;
	unsigned long svpfn;
	unsigned long start_vaddr;
	unsigned long end_vaddr;
	int copied = 0;

	if (!task)
		goto out;

	mm = mm_for_maps(task);
	ret = PTR_ERR(mm);
	if (!mm || IS_ERR(mm))
		goto out_task;

	ret = -EINVAL;
	 
	if ((*ppos % PM_ENTRY_BYTES) || (count % PM_ENTRY_BYTES))
		goto out_task;

	ret = 0;

	if (!count)
		goto out_task;

	pm.len = PM_ENTRY_BYTES * (PAGEMAP_WALK_SIZE >> PAGE_SHIFT);
	pm.buffer = kmalloc(pm.len, GFP_TEMPORARY);
	ret = -ENOMEM;
	if (!pm.buffer)
		goto out_mm;

	pagemap_walk.pmd_entry = pagemap_pte_range;
	pagemap_walk.pte_hole = pagemap_pte_hole;
#ifdef CONFIG_HUGETLB_PAGE
	pagemap_walk.hugetlb_entry = pagemap_hugetlb_range;
#endif
	pagemap_walk.mm = mm;
	pagemap_walk.private = &pm;

	src = *ppos;
	svpfn = src / PM_ENTRY_BYTES;
	start_vaddr = svpfn << PAGE_SHIFT;
	end_vaddr = TASK_SIZE_OF(task);

	 
	if (svpfn > TASK_SIZE_OF(task) >> PAGE_SHIFT)
		start_vaddr = end_vaddr;

	 
	ret = 0;
	while (count && (start_vaddr < end_vaddr)) {
		int len;
		unsigned long end;

		pm.pos = 0;
		end = (start_vaddr + PAGEMAP_WALK_SIZE) & PAGEMAP_WALK_MASK;
		 
		if (end < start_vaddr || end > end_vaddr)
			end = end_vaddr;
		down_read(&mm->mmap_sem);
		ret = walk_page_range(start_vaddr, end, &pagemap_walk);
		up_read(&mm->mmap_sem);
		start_vaddr = end;

		len = min(count, PM_ENTRY_BYTES * pm.pos);
		if (copy_to_user(buf, pm.buffer, len)) {
			ret = -EFAULT;
			goto out_free;
		}
		copied += len;
		buf += len;
		count -= len;
	}
	*ppos += copied;
	if (!ret || ret == PM_END_OF_BUFFER)
		ret = copied;

out_free:
	kfree(pm.buffer);
out_mm:
	mmput(mm);
out_task:
	put_task_struct(task);
out:
	return ret;
}