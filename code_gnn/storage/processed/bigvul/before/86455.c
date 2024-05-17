int handle_userfault(struct vm_fault *vmf, unsigned long reason)
{
	struct mm_struct *mm = vmf->vma->vm_mm;
	struct userfaultfd_ctx *ctx;
	struct userfaultfd_wait_queue uwq;
	int ret;
	bool must_wait, return_to_userland;
	long blocking_state;

	ret = VM_FAULT_SIGBUS;

	 
	if (current->flags & (PF_EXITING|PF_DUMPCORE))
		goto out;

	 
	WARN_ON_ONCE(!rwsem_is_locked(&mm->mmap_sem));

	ctx = vmf->vma->vm_userfaultfd_ctx.ctx;
	if (!ctx)
		goto out;

	BUG_ON(ctx->mm != mm);

	VM_BUG_ON(reason & ~(VM_UFFD_MISSING|VM_UFFD_WP));
	VM_BUG_ON(!(reason & VM_UFFD_MISSING) ^ !!(reason & VM_UFFD_WP));

	if (ctx->features & UFFD_FEATURE_SIGBUS)
		goto out;

	 
	if (unlikely(ACCESS_ONCE(ctx->released))) {
		 
		ret = VM_FAULT_NOPAGE;
		goto out;
	}

	 
	if (unlikely(!(vmf->flags & FAULT_FLAG_ALLOW_RETRY))) {
		 
		BUG_ON(vmf->flags & FAULT_FLAG_RETRY_NOWAIT);
#ifdef CONFIG_DEBUG_VM
		if (printk_ratelimit()) {
			printk(KERN_WARNING
			       "FAULT_FLAG_ALLOW_RETRY missing %x\n",
			       vmf->flags);
			dump_stack();
		}
#endif
		goto out;
	}

	 
	ret = VM_FAULT_RETRY;
	if (vmf->flags & FAULT_FLAG_RETRY_NOWAIT)
		goto out;

	 
	userfaultfd_ctx_get(ctx);

	init_waitqueue_func_entry(&uwq.wq, userfaultfd_wake_function);
	uwq.wq.private = current;
	uwq.msg = userfault_msg(vmf->address, vmf->flags, reason,
			ctx->features);
	uwq.ctx = ctx;
	uwq.waken = false;

	return_to_userland =
		(vmf->flags & (FAULT_FLAG_USER|FAULT_FLAG_KILLABLE)) ==
		(FAULT_FLAG_USER|FAULT_FLAG_KILLABLE);
	blocking_state = return_to_userland ? TASK_INTERRUPTIBLE :
			 TASK_KILLABLE;

	spin_lock(&ctx->fault_pending_wqh.lock);
	 
	__add_wait_queue(&ctx->fault_pending_wqh, &uwq.wq);
	 
	set_current_state(blocking_state);
	spin_unlock(&ctx->fault_pending_wqh.lock);

	if (!is_vm_hugetlb_page(vmf->vma))
		must_wait = userfaultfd_must_wait(ctx, vmf->address, vmf->flags,
						  reason);
	else
		must_wait = userfaultfd_huge_must_wait(ctx, vmf->vma,
						       vmf->address,
						       vmf->flags, reason);
	up_read(&mm->mmap_sem);

	if (likely(must_wait && !ACCESS_ONCE(ctx->released) &&
		   (return_to_userland ? !signal_pending(current) :
		    !fatal_signal_pending(current)))) {
		wake_up_poll(&ctx->fd_wqh, POLLIN);
		schedule();
		ret |= VM_FAULT_MAJOR;

		 
		while (!READ_ONCE(uwq.waken)) {
			 
			set_current_state(blocking_state);
			if (READ_ONCE(uwq.waken) ||
			    READ_ONCE(ctx->released) ||
			    (return_to_userland ? signal_pending(current) :
			     fatal_signal_pending(current)))
				break;
			schedule();
		}
	}

	__set_current_state(TASK_RUNNING);

	if (return_to_userland) {
		if (signal_pending(current) &&
		    !fatal_signal_pending(current)) {
			 
			down_read(&mm->mmap_sem);
			ret = VM_FAULT_NOPAGE;
		}
	}

	 
	if (!list_empty_careful(&uwq.wq.entry)) {
		spin_lock(&ctx->fault_pending_wqh.lock);
		 
		list_del(&uwq.wq.entry);
		spin_unlock(&ctx->fault_pending_wqh.lock);
	}

	 
	userfaultfd_ctx_put(ctx);

out:
	return ret;
}
