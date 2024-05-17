 static int userfaultfd_release(struct inode *inode, struct file *file)
 {
 	struct userfaultfd_ctx *ctx = file->private_data;
 	struct mm_struct *mm = ctx->mm;
 	struct vm_area_struct *vma, *prev;
 	 
 	struct userfaultfd_wake_range range = { .len = 0, };
 	unsigned long new_flags;
 
 	WRITE_ONCE(ctx->released, true);
 
 	if (!mmget_not_zero(mm))
 		goto wakeup;
 
 	 
  	down_write(&mm->mmap_sem);
// 	if (!mmget_still_valid(mm))
// 		goto skip_mm;
  	prev = NULL;
  	for (vma = mm->mmap; vma; vma = vma->vm_next) {
  		cond_resched();
 		BUG_ON(!!vma->vm_userfaultfd_ctx.ctx ^
 		       !!(vma->vm_flags & (VM_UFFD_MISSING | VM_UFFD_WP)));
 		if (vma->vm_userfaultfd_ctx.ctx != ctx) {
 			prev = vma;
 			continue;
 		}
 		new_flags = vma->vm_flags & ~(VM_UFFD_MISSING | VM_UFFD_WP);
 		prev = vma_merge(mm, prev, vma->vm_start, vma->vm_end,
 				 new_flags, vma->anon_vma,
 				 vma->vm_file, vma->vm_pgoff,
 				 vma_policy(vma),
 				 NULL_VM_UFFD_CTX);
 		if (prev)
 			vma = prev;
 		else
 			prev = vma;
  		vma->vm_flags = new_flags;
  		vma->vm_userfaultfd_ctx = NULL_VM_UFFD_CTX;
  	}
// skip_mm:
  	up_write(&mm->mmap_sem);
  	mmput(mm);
  wakeup:
 	 
 	spin_lock(&ctx->fault_pending_wqh.lock);
 	__wake_up_locked_key(&ctx->fault_pending_wqh, TASK_NORMAL, &range);
 	__wake_up(&ctx->fault_wqh, TASK_NORMAL, 1, &range);
 	spin_unlock(&ctx->fault_pending_wqh.lock);
 
 	 
 	wake_up_all(&ctx->event_wqh);
 
 	wake_up_poll(&ctx->fd_wqh, EPOLLHUP);
 	userfaultfd_ctx_put(ctx);
 	return 0;
 }