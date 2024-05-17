static int show_smap(struct seq_file *m, void *v)
{
	struct proc_maps_private *priv = m->private;
	struct task_struct *task = priv->task;
	struct vm_area_struct *vma = v;
	struct mem_size_stats mss;
	struct mm_walk smaps_walk = {
		.pmd_entry = smaps_pte_range,
		.mm = vma->vm_mm,
		.private = &mss,
	};

	memset(&mss, 0, sizeof mss);
	mss.vma = vma;
	 
	if (vma->vm_mm && !is_vm_hugetlb_page(vma))
		walk_page_range(vma->vm_start, vma->vm_end, &smaps_walk);

	show_map_vma(m, vma);

	seq_printf(m,
		   "Size:           %8lu kB\n"
		   "Rss:            %8lu kB\n"
		   "Pss:            %8lu kB\n"
		   "Shared_Clean:   %8lu kB\n"
		   "Shared_Dirty:   %8lu kB\n"
		   "Private_Clean:  %8lu kB\n"
		   "Private_Dirty:  %8lu kB\n"
		   "Referenced:     %8lu kB\n"
		   "Anonymous:      %8lu kB\n"
		   "AnonHugePages:  %8lu kB\n"
		   "Swap:           %8lu kB\n"
		   "KernelPageSize: %8lu kB\n"
		   "MMUPageSize:    %8lu kB\n"
		   "Locked:         %8lu kB\n",
		   (vma->vm_end - vma->vm_start) >> 10,
		   mss.resident >> 10,
		   (unsigned long)(mss.pss >> (10 + PSS_SHIFT)),
		   mss.shared_clean  >> 10,
		   mss.shared_dirty  >> 10,
		   mss.private_clean >> 10,
		   mss.private_dirty >> 10,
		   mss.referenced >> 10,
		   mss.anonymous >> 10,
		   mss.anonymous_thp >> 10,
		   mss.swap >> 10,
		   vma_kernel_pagesize(vma) >> 10,
		   vma_mmu_pagesize(vma) >> 10,
		   (vma->vm_flags & VM_LOCKED) ?
			(unsigned long)(mss.pss >> (10 + PSS_SHIFT)) : 0);

	if (m->count < m->size)   
		m->version = (vma != get_gate_vma(task->mm))
			? vma->vm_start : 0;
	return 0;
}