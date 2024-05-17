static int walk_page_test(unsigned long start, unsigned long end,
			struct mm_walk *walk)
{
	struct vm_area_struct *vma = walk->vma;

	if (walk->test_walk)
		return walk->test_walk(start, end, walk);

	 
	if (vma->vm_flags & VM_PFNMAP) {
		int err = 1;
		if (walk->pte_hole)
			err = walk->pte_hole(start, end, walk);
		return err ? err : 1;
	}
	return 0;
}
