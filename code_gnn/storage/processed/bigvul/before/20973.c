int sys_vm86(unsigned long cmd, unsigned long arg, struct pt_regs *regs)
{
	struct kernel_vm86_struct info;  
	struct task_struct *tsk;
	int tmp, ret;
	struct vm86plus_struct __user *v86;

	tsk = current;
	switch (cmd) {
	case VM86_REQUEST_IRQ:
	case VM86_FREE_IRQ:
	case VM86_GET_IRQ_BITS:
	case VM86_GET_AND_RESET_IRQ:
		ret = do_vm86_irq_handling(cmd, (int)arg);
		goto out;
	case VM86_PLUS_INSTALL_CHECK:
		 
		ret = 0;
		goto out;
	}

	 
	ret = -EPERM;
	if (tsk->thread.saved_sp0)
		goto out;
	v86 = (struct vm86plus_struct __user *)arg;
	tmp = copy_vm86_regs_from_user(&info.regs, &v86->regs,
				       offsetof(struct kernel_vm86_struct, regs32) -
				       sizeof(info.regs));
	ret = -EFAULT;
	if (tmp)
		goto out;
	info.regs32 = regs;
	info.vm86plus.is_vm86pus = 1;
	tsk->thread.vm86_info = (struct vm86_struct __user *)v86;
	do_sys_vm86(&info, tsk);
	ret = 0;	 
out:
	return ret;
}