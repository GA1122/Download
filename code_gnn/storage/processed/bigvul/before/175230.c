static int prctl_set_mm(int opt, unsigned long addr,
 unsigned long arg4, unsigned long arg5)
{
 unsigned long rlim = rlimit(RLIMIT_DATA);
 unsigned long vm_req_flags;
 unsigned long vm_bad_flags;
 struct vm_area_struct *vma;
 int error = 0;
 struct mm_struct *mm = current->mm;

 if (arg4 | arg5)
 return -EINVAL;

 if (!capable(CAP_SYS_RESOURCE))
 return -EPERM;

 if (addr >= TASK_SIZE)
 return -EINVAL;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, addr);

 if (opt != PR_SET_MM_START_BRK && opt != PR_SET_MM_BRK) {
  
 if (!vma || vma->vm_start > addr)
 goto out;
 }

	error = -EINVAL;
 switch (opt) {
 case PR_SET_MM_START_CODE:
 case PR_SET_MM_END_CODE:
		vm_req_flags = VM_READ | VM_EXEC;
		vm_bad_flags = VM_WRITE | VM_MAYSHARE;

 if ((vma->vm_flags & vm_req_flags) != vm_req_flags ||
 (vma->vm_flags & vm_bad_flags))
 goto out;

 if (opt == PR_SET_MM_START_CODE)
			mm->start_code = addr;
 else
			mm->end_code = addr;
 break;

 case PR_SET_MM_START_DATA:
 case PR_SET_MM_END_DATA:
		vm_req_flags = VM_READ | VM_WRITE;
		vm_bad_flags = VM_EXEC | VM_MAYSHARE;

 if ((vma->vm_flags & vm_req_flags) != vm_req_flags ||
 (vma->vm_flags & vm_bad_flags))
 goto out;

 if (opt == PR_SET_MM_START_DATA)
			mm->start_data = addr;
 else
			mm->end_data = addr;
 break;

 case PR_SET_MM_START_STACK:

#ifdef CONFIG_STACK_GROWSUP
		vm_req_flags = VM_READ | VM_WRITE | VM_GROWSUP;
#else
		vm_req_flags = VM_READ | VM_WRITE | VM_GROWSDOWN;
#endif
 if ((vma->vm_flags & vm_req_flags) != vm_req_flags)
 goto out;

		mm->start_stack = addr;
 break;

 case PR_SET_MM_START_BRK:
 if (addr <= mm->end_data)
 goto out;

 if (rlim < RLIM_INFINITY &&
 (mm->brk - addr) +
 (mm->end_data - mm->start_data) > rlim)
 goto out;

		mm->start_brk = addr;
 break;

 case PR_SET_MM_BRK:
 if (addr <= mm->end_data)
 goto out;

 if (rlim < RLIM_INFINITY &&
 (addr - mm->start_brk) +
 (mm->end_data - mm->start_data) > rlim)
 goto out;

		mm->brk = addr;
 break;

 default:
		error = -EINVAL;
 goto out;
 }

	error = 0;

out:
	up_read(&mm->mmap_sem);

 return error;
}
