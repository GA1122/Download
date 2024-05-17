static int copy_vm86_regs_from_user(struct kernel_vm86_regs *regs,
				    const struct vm86_regs __user *user,
				    unsigned extra)
{
	int ret = 0;

	 
	ret += copy_from_user(regs, user, offsetof(struct kernel_vm86_regs, pt.orig_ax));
	 
	ret += copy_from_user(&regs->pt.orig_ax, &user->orig_eax,
			      sizeof(struct kernel_vm86_regs) -
			      offsetof(struct kernel_vm86_regs, pt.orig_ax) +
			      extra);
	return ret;
}