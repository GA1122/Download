hugetlb_vmtruncate_list(struct prio_tree_root *root, pgoff_t pgoff)
{
	struct vm_area_struct *vma;
	struct prio_tree_iter iter;

	vma_prio_tree_foreach(vma, &iter, root, pgoff, ULONG_MAX) {
		unsigned long v_offset;

		 
		if (vma->vm_pgoff < pgoff)
			v_offset = (pgoff - vma->vm_pgoff) << PAGE_SHIFT;
		else
			v_offset = 0;

		__unmap_hugepage_range(vma,
				vma->vm_start + v_offset, vma->vm_end, NULL);
	}
}
