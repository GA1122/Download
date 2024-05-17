static int prctl_set_vma(unsigned long opt, unsigned long start,
 unsigned long len_in, unsigned long arg)
{
 struct mm_struct *mm = current->mm;
 int error;
 unsigned long len;
 unsigned long end;

 if (start & ~PAGE_MASK)
 return -EINVAL;
	len = (len_in + ~PAGE_MASK) & PAGE_MASK;

  
 if (len_in && !len)
 return -EINVAL;

	end = start + len;
 if (end < start)
 return -EINVAL;

 if (end == start)
 return 0;

	down_write(&mm->mmap_sem);

 switch (opt) {
 case PR_SET_VMA_ANON_NAME:
		error = prctl_set_vma_anon_name(start, end, arg);
 break;
 default:
		error = -EINVAL;
 }

	up_write(&mm->mmap_sem);

 return error;
}
