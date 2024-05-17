unsigned long unmapped_area(struct vm_unmapped_area_info *info)
{
	 

	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long length, low_limit, high_limit, gap_start, gap_end;

	 
	length = info->length + info->align_mask;
	if (length < info->length)
		return -ENOMEM;

	 
	if (info->high_limit < length)
		return -ENOMEM;
	high_limit = info->high_limit - length;

	if (info->low_limit > high_limit)
		return -ENOMEM;
	low_limit = info->low_limit + length;

	 
	if (RB_EMPTY_ROOT(&mm->mm_rb))
		goto check_highest;
	vma = rb_entry(mm->mm_rb.rb_node, struct vm_area_struct, vm_rb);
	if (vma->rb_subtree_gap < length)
		goto check_highest;

	while (true) {
		 
		gap_end = vm_start_gap(vma);
		if (gap_end >= low_limit && vma->vm_rb.rb_left) {
			struct vm_area_struct *left =
				rb_entry(vma->vm_rb.rb_left,
					 struct vm_area_struct, vm_rb);
			if (left->rb_subtree_gap >= length) {
				vma = left;
				continue;
			}
		}

		gap_start = vma->vm_prev ? vm_end_gap(vma->vm_prev) : 0;
check_current:
		 
		if (gap_start > high_limit)
			return -ENOMEM;
		if (gap_end >= low_limit &&
		    gap_end > gap_start && gap_end - gap_start >= length)
			goto found;

		 
		if (vma->vm_rb.rb_right) {
			struct vm_area_struct *right =
				rb_entry(vma->vm_rb.rb_right,
					 struct vm_area_struct, vm_rb);
			if (right->rb_subtree_gap >= length) {
				vma = right;
				continue;
			}
		}

		 
		while (true) {
			struct rb_node *prev = &vma->vm_rb;
			if (!rb_parent(prev))
				goto check_highest;
			vma = rb_entry(rb_parent(prev),
				       struct vm_area_struct, vm_rb);
			if (prev == vma->vm_rb.rb_left) {
				gap_start = vm_end_gap(vma->vm_prev);
				gap_end = vm_start_gap(vma);
				goto check_current;
			}
		}
	}

check_highest:
	 
	gap_start = mm->highest_vm_end;
	gap_end = ULONG_MAX;   
	if (gap_start > high_limit)
		return -ENOMEM;

found:
	 
	if (gap_start < info->low_limit)
		gap_start = info->low_limit;

	 
	gap_start += (info->align_offset - gap_start) & info->align_mask;

	VM_BUG_ON(gap_start + info->length > info->high_limit);
	VM_BUG_ON(gap_start + info->length > gap_end);
	return gap_start;
}
