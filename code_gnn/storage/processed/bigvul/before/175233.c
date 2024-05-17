static int prctl_update_vma_anon_name(struct vm_area_struct *vma,
 struct vm_area_struct **prev,
 unsigned long start, unsigned long end,
 const char __user *name_addr)
{
 struct mm_struct * mm = vma->vm_mm;
 int error = 0;
 pgoff_t pgoff;

 if (name_addr == vma_get_anon_name(vma)) {
 *prev = vma;
 goto out;
 }

	pgoff = vma->vm_pgoff + ((start - vma->vm_start) >> PAGE_SHIFT);
 *prev = vma_merge(mm, *prev, start, end, vma->vm_flags, vma->anon_vma,
				vma->vm_file, pgoff, vma_policy(vma),
				name_addr);
 if (*prev) {
		vma = *prev;
 goto success;
 }

 *prev = vma;

 if (start != vma->vm_start) {
		error = split_vma(mm, vma, start, 1);
 if (error)
 goto out;
 }

 if (end != vma->vm_end) {
		error = split_vma(mm, vma, end, 0);
 if (error)
 goto out;
 }

success:
 if (!vma->vm_file)
		vma->shared.anon_name = name_addr;

out:
 if (error == -ENOMEM)
		error = -EAGAIN;
 return error;
}
