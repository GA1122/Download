static int copy_vm86_regs_to_user(struct vm86_regs __user *user,
				  const struct kernel_vm86_regs *regs)
{
	int ret = 0;

	 
	ret += copy_to_user(user, regs, offsetof(struct kernel_vm86_regs, pt.orig_ax));
	ret += copy_to_user(&user->orig_eax, &regs->pt.orig_ax,
			    sizeof(struct kernel_vm86_regs) -
			    offsetof(struct kernel_vm86_regs, pt.orig_ax));

	return ret;
}